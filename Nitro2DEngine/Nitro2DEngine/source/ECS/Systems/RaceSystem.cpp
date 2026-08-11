#include "ECS/Systems/RaceSystem.h"

#include "ECS/Registry.h"

#include "ECS/Components/RaceManagerComponent.h"
#include "ECS/Components/RaceParticipantComponent.h"
#include "ECS/Components/MovementControlComponent.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/StartingGridComponent.h"
#include "ECS/Components/PathComponent.h"
#include "Modules/Math2D.h"

#include <algorithm>
#include <vector>
#include <random>

namespace
{
  struct RaceRankingEntry
  {
    ECS::EntityID entity{
        ECS::NULL_ENTITY
    };

    int completedLaps{
        0
    };

    float lapProgress{
        0.f
    };
  };
}

namespace ECS
{
  void 
  RaceSystem::OnUpdate(Registry& registry, float deltaTime)
  {
    (void)deltaTime;

    registry
      .GetView<RaceParticipantComponent,
      MovementControlComponent>()
      .Each(
        [&registry](
          EntityID entity,
          RaceParticipantComponent& participant,
          MovementControlComponent& movement)
        {
          // ----------------------------------
          // Sin RaceManager:
          // RaceSystem no controla esta entidad.
          // ----------------------------------

          if (participant.raceManagerEntity == NULL_ENTITY) return;


          if (!registry.IsAlive(participant.raceManagerEntity))
          {
            movement.enabled = false;
            return;
          }


          const RaceManagerComponent* manager =
            registry.TryGetComponent<
            RaceManagerComponent>(
              participant.raceManagerEntity);

          if (!manager)
          {
            movement.enabled =false;

            return;
          }


          // ----------------------------------
          // RACING
          // ----------------------------------

          if (manager->state == RaceState::Racing)
          {
            movement.enabled = true;

            return;
          }


          // ----------------------------------
          // WAITING / FINISHED
          // ----------------------------------

          movement.enabled = false;


          // Limpiamos cualquier movimiento residual.
          if (auto* velocity = registry.TryGetComponent<Velocity>(entity))
          {
            velocity->velocity = { 0.f, 0.f };
          }

          if (auto* acceleration = registry.TryGetComponent<Acceleration>(entity))
          {
            acceleration->acceleration = { 0.f, 0.f };
          }

          
        });

  // ======================================================
  // RACE PROGRESS
  // ======================================================


    registry
      .GetView<RaceManagerComponent>()
      .Each(
        [&registry](
          EntityID raceEntity,
          RaceManagerComponent& race)
        {
          // El progreso solo cambia mientras
          // la carrera está en marcha.
          if (race.state !=
            RaceState::Racing)
          {
            return;
          }

          std::vector<RaceRankingEntry> rankingEntries;

          // ------------------------------------------
          // Obtener Starting Grid
          // ------------------------------------------

          if (race.startingGridEntity ==
            NULL_ENTITY ||
            !registry.IsAlive(
              race.startingGridEntity))
          {
            return;
          }

          const StartingGridComponent* grid =
            registry.TryGetComponent<
            StartingGridComponent>(
              race.startingGridEntity);

          if (!grid)
          {
            return;
          }

          // ------------------------------------------
          // Obtener Path
          // ------------------------------------------

          if (grid->pathEntity ==
            NULL_ENTITY ||
            !registry.IsAlive(
              grid->pathEntity))
          {
            return;
          }

          const PathComponent* path =
            registry.TryGetComponent<
            PathComponent>(
              grid->pathEntity);

          if (!path ||
            path->points.size() < 2)
          {
            return;
          }


          // ==========================================
          // PARTICIPANTS
          // ==========================================

          registry
            .GetView<
            Transform,
            RaceParticipantComponent>()
            .Each(
              [&](
                EntityID participantEntity,
                Transform& transform,
                RaceParticipantComponent& participant)
              {
                if (participant.raceManagerEntity !=
                  raceEntity)
                {
                  return;
                }


                // ----------------------------------
                // Medir progreso
                // ----------------------------------

                const Math::PathProgressResult progress =
                  Math::ComputePathProgress(
                    path->points,
                    path->closed,
                    transform.position);

                const float currentProgress =
                  progress.normalizedProgress;


                // ----------------------------------
                // Primera muestra
                // ----------------------------------

                if (!participant.progressInitialized)
                {
                  participant.lapProgress =
                    currentProgress;

                  participant.previousLapProgress =
                    currentProgress;

                  participant.progressInitialized =
                    true;
                }
                else
                {
                  participant.lapProgress =
                    currentProgress;


                  if (!participant.lapCountingArmed &&
                    currentProgress > 0.25f &&
                    currentProgress < 0.75f)
                  {
                    participant.lapCountingArmed =
                      true;
                  }


                  const bool crossedFinishLine =
                    participant.lapCountingArmed &&
                    participant.previousLapProgress >
                    0.75f &&
                    currentProgress <
                    0.25f;

                  if (crossedFinishLine)
                  {
                    ++participant.completedLaps;

                    participant.lapCountingArmed =
                      false;
                  }


                  participant.previousLapProgress =
                    currentProgress;
                }

                rankingEntries.push_back({ participantEntity, participant.completedLaps, participant.lapProgress});

              });

              // ======================================================
              // LEADERBOARD SORT
              // ======================================================

              std::sort(rankingEntries.begin(), rankingEntries.end(),
                [](
                  const RaceRankingEntry& a,
                  const RaceRankingEntry& b)
                {
                  // Primero: más vueltas completadas.
                  if (a.completedLaps != b.completedLaps)
                  {
                    return a.completedLaps > b.completedLaps;
                  }

                  // Segundo: mayor progreso de vuelta.
                  return a.lapProgress > b.lapProgress;
                });

              for (std::size_t i = 0;
                i < rankingEntries.size();
                ++i)
              {
                RaceParticipantComponent* participant =
                  registry.TryGetComponent<
                  RaceParticipantComponent>(
                    rankingEntries[i].entity);

                if (!participant)
                {
                  continue;
                }

                participant->racePosition =
                  static_cast<int>(i) + 1;
              }
        });
  }

}
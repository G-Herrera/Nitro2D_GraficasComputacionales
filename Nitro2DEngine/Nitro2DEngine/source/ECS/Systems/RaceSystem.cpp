#include "ECS/Systems/RaceSystem.h"

#include "ECS/Registry.h"

#include "ECS/Components/RaceManagerComponent.h"
#include "ECS/Components/RaceParticipantComponent.h"
#include "ECS/Components/MovementControlComponent.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"

#include <random>
#include <algorithm>

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
  }

}
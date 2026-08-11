#pragma once

#include "Prerequisites.h"

#include "ECS/Registry.h"

#include "ECS/Components/RaceManagerComponent.h"
#include "ECS/Components/RaceParticipantComponent.h"
#include "ECS/Components/SteeringComponent.h"

#include <algorithm>
#include <random>

namespace ECS
{
  inline void 
  RandomizeRaceParticipants(Registry& registry, EntityID raceManagerEntity)
  {
    if (raceManagerEntity == NULL_ENTITY) return;

    if (!registry.IsAlive(raceManagerEntity)) return;


    auto* race = registry.TryGetComponent<RaceManagerComponent>(raceManagerEntity);

    if (!race) return;


    const float minSpeed = std::min(race->minRandomSpeed, race->maxRandomSpeed);

    const float maxSpeed = std::max(race->minRandomSpeed, race->maxRandomSpeed);


    static std::mt19937 generator{std::random_device{}()};

    std::uniform_real_distribution<float> speedDistribution(minSpeed, maxSpeed);


    registry
      .GetView<
      RaceParticipantComponent,
      SteeringComponent>()
      .Each(
        [&](EntityID entity,
          RaceParticipantComponent& participant,
          SteeringComponent& steering)
        {
          (void)entity;

          if (participant.raceManagerEntity != raceManagerEntity) return;

          steering.maxSpeed = speedDistribution(generator);
        });
  }
}
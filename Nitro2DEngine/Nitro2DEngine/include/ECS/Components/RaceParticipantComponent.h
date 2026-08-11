#pragma once

#include "Prerequisites.h"
#include "ECS/Types.h"

namespace ECS
{
  struct RaceParticipantComponent
  {
    // Parrilla a la que pertenece.
    EntityID startingGridEntity{
        NULL_ENTITY
    };

    // -1 = sin posición asignada.
    // 0 = Slot 1
    // 1 = Slot 2
    // etc.
    int startingSlot{ -1 };

    RaceParticipantComponent() = default;
  };
}
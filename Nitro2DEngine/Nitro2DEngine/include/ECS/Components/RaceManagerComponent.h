#pragma once

#include "Prerequisites.h"
#include "ECS/Types.h"

namespace ECS
{
  enum class RaceState
  {
    Waiting = 0,
    Racing,
    Finished
  };


  struct RaceManagerComponent
  {
    // Estado global de esta carrera.
    RaceState state{
        RaceState::Waiting
    };

    // Parrilla asociada a esta carrera.
    EntityID startingGridEntity{
        NULL_ENTITY
    };

    // --------------------------------------------------
    // Randomization
    // --------------------------------------------------
    
    // Rango permitido para randomizar maxSpeed
    // de los participantes.
    float minRandomSpeed{ 90.f };
    float maxRandomSpeed{ 120.f };

    RaceManagerComponent() = default;
  };
}
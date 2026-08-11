#pragma once

#include "Prerequisites.h"
#include "ECS/Types.h"

namespace ECS
{
  struct RaceParticipantComponent
  {
    // Carrera a la que pertenece este participante.
    EntityID raceManagerEntity{
        NULL_ENTITY
    };

    // Parrilla a la que pertenece.
    EntityID startingGridEntity{
        NULL_ENTITY
    };

    // -1 = sin posición asignada.
    // 0 = Slot 1
    // 1 = Slot 2
    // etc.
    int startingSlot{ -1 };

    // --------------------------------------------------
    // Race Progress
    // --------------------------------------------------
    
    // Vueltas COMPLETADAS.
    // Empieza en 0.
    int completedLaps{0};

    // Progreso de la vuelta actual [0..1].
    float lapProgress{0.f};

    // Progreso del frame anterior.
    // Se utiliza para detectar el wrap 1 -> 0.
    float previousLapProgress{0.f};

    // Posición actual en carrera:
    // 1 = primero, 2 = segundo, etc.
    int racePosition{0};

    // Evita contar el cruce inicial de salida
    // como una vuelta completa.
    bool lapCountingArmed{false};

    // Permite saber si ya hemos inicializado
    // previousLapProgress.
    bool progressInitialized{false};

    RaceParticipantComponent() = default;
  };
}
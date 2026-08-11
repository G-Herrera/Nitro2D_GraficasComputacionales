#pragma once

// ======================================================
// ECS :: Components/MovementControlComponent.h
//
// Gate genérico que permite habilitar o bloquear
// el movimiento de una entidad.
//
// No conoce RaceSystem, SteeringSystem ni estados
// específicos del juego.
//
// ======================================================

namespace ECS
{
  struct MovementControlComponent
  {
    bool enabled{ true };

    MovementControlComponent() = default;
  };
}
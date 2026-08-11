#pragma once

#include "Prerequisites.h"
#include "ECS/System.h"

namespace ECS
{
  class Registry;

  // ======================================================
  // ECS :: RaceSystem
  //
  // Coordina el estado global de las carreras.
  //
  // Responsabilidades actuales:
  // - Leer RaceManagerComponent.
  // - Habilitar o bloquear participantes.
  // - Mantener los karts quietos durante Waiting.
  //
  // NO calcula Steering.
  // NO modifica Path Following.
  // NO renderiza.
  // ======================================================

  class 
  RaceSystem final : public System
  {
  public:
    RaceSystem() = default;
    ~RaceSystem() override = default;

    void 
    OnUpdate(Registry& registry, float deltaTime) override;

  };
}
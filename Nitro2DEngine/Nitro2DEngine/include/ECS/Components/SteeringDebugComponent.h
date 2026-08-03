#pragma once
#include "Prerequisites.h"

// ======================================================
// ECS :: Components/SteeringDebugComponent.h
//
// Componente de DATOS PUROS para almacenar informacion
// visual y temporal producida por SteeringSystem.
//
// SteeringSystem escribe estos valores durante OnUpdate.
// DebugRenderSystem los consulta y los dibuja.
//
// Este componente NO calcula steering y NO renderiza.
// ======================================================

namespace ECS
{
  struct SteeringDebugComponent
  {
    // Interruptor principal del debug de este agente.
    bool enabled{ true };

    // Elementos que se pueden mostrar individualmente.
    bool drawVelocity{ true };
    bool drawPredictedPosition{ true };
    bool drawNearestPathPoint{ true };
    bool drawPathTargetPoint{ true };
    bool drawPathFollowingForce{ true };
    bool drawSeparationForce{ true };
    bool drawFinalSteeringForce{ true };

    // Multiplicadores visuales.
    // No modifican el comportamiento del agente.
    float velocityScale{ 0.5f };
    float forceScale{ 1.f };

    // ------------------------------------------------------
    // Colores configurables del debug
    // ------------------------------------------------------

    sf::Color velocityColor{0, 255, 0, 230};

    sf::Color predictedPositionColor{255, 255, 255, 230};

    sf::Color nearestPathPointColor{255, 0, 255, 230};

    sf::Color pathTargetPointColor{255, 50, 50, 230};

    sf::Color pathFollowingForceColor{0, 150, 255, 230};

    sf::Color separationForceColor{180, 80, 255, 230};

    sf::Color finalSteeringForceColor{255, 140, 0, 230  };

    // Estado actual del agente.
    sf::Vector2f velocity{ 0.f, 0.f };

    // Datos calculados por Path Following.
    sf::Vector2f predictedPosition{ 0.f, 0.f };
    sf::Vector2f nearestPathPoint{ 0.f, 0.f };
    sf::Vector2f pathTargetPoint{ 0.f, 0.f };

    // Fuerzas individuales producidas por comportamientos.
    sf::Vector2f pathFollowingForce{ 0.f, 0.f };
    sf::Vector2f separationForce{ 0.f, 0.f };

    // Fuerza resultante después del sistema de prioridades.
    sf::Vector2f finalSteeringForce{ 0.f, 0.f };

    SteeringDebugComponent() = default;
  };
}
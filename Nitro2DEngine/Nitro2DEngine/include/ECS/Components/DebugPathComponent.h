#pragma once
#include "Prerequisites.h"

// ======================================================
// ECS :: Components/DebugPathComponent.h
//
// Configuracion de DATOS PUROS para visualizar un Path.
//
// No contiene logica de render. DebugRenderSystem consulta
// estos valores para decidir que elementos dibujar.
// ======================================================

namespace ECS
{
  struct DebugPathComponent
  {
    // Interruptor principal.
    bool enabled{ true };

    // Dibuja la linea central de la polilinea.
    bool drawCenterLine{ true };

    // Dibuja los dos limites aproximados del corredor.
    bool drawPathRadius{ true };

    // Dibuja un pequeño marcador en algunos puntos.
    bool drawSamplePoints{ false };

    // Evita dibujar los 200 puntos como marcadores.
    int samplePointStep{ 10 };

    // Colores configurables.
    sf::Color centerLineColor{
        255, 255, 0, 220
    };

    sf::Color radiusLineColor{
        0, 255, 255, 150
    };

    sf::Color samplePointColor{
        255, 100, 100, 220
    };

    DebugPathComponent() = default;
  };
}
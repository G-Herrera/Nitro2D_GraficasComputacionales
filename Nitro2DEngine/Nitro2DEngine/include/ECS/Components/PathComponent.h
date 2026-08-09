#pragma once
#include "Prerequisites.h"

// ======================================================
// ECS :: Components/PathComponent.h
//
// PathComponent - componente de DATOS PUROS (sin logica).
// Representa un camino cerrado (circuito) como una polilinea
// DENSA ya muestreada (ver Math::BuildClosedCatmullRom), lista
// para que SteeringSystem haga geometria de segmento de linea
// (proyeccion, punto mas cercano, punto adelante) sin tener que
// evaluar una curva en tiempo real.
//
// Se agrega a UNA entidad que representa el circuito (no a cada
// kart). Los karts solo guardan una referencia (EntityID) a esa
// entidad, igual que ya hace SteeringComponent::target.
// ======================================================
namespace ECS {
  struct PathComponent
  {
    // --------------------------------------------------
    // Datos editables
    // --------------------------------------------------

    // Puntos principales colocados por el diseñador.
    // Son la fuente de verdad del path.
    std::vector<sf::Vector2f> controlPoints;

    // Cantidad de muestras Catmull-Rom generadas entre
    // cada pareja de puntos de control.
    int samplesPerSegment{ 20 };

    // Determina si el camino vuelve al primer punto.
    bool closed{ true };

    // Radio visual/lógico del corredor.
    float radius{ 40.f };


    // --------------------------------------------------
    // Datos derivados
    // --------------------------------------------------

    // Polilínea densa utilizada en runtime por
    // Path Following.
    //
    // NO debe editarse manualmente desde el editor.
    std::vector<sf::Vector2f> points;


    PathComponent() = default;
  };
}
#pragma once

#include "Prerequisites.h"
#include "ECS/Types.h"

namespace ECS
{
  struct StartingGridSlot
  {
    sf::Vector2f position{ 0.f, 0.f };
    float rotation{ 0.f };
  };

  struct StartingGridComponent
  {
    // Path que define la dirección de carrera.
    EntityID pathEntity{ NULL_ENTITY };

    // Punto sampled del path donde estará la meta.
    int anchorPointIndex{ 0 };

    // Mitad de longitud visual de la línea de meta.
    float lineHalfWidth{ 90.f };

    // Cantidad de posiciones disponibles.
    int slotCount{ 4 };

    // Distancia desde la meta hasta la primera fila.
    float firstRowOffset{ 45.f };

    // Separación longitudinal entre filas.
    float rowSpacing{ 45.f };

    // Separación lateral entre los dos karts de una fila.
    float columnSpacing{ 40.f };

    // Datos derivados.
    sf::Vector2f finishLineCenter{ 0.f, 0.f };
    sf::Vector2f finishLineStart{ 0.f, 0.f };
    sf::Vector2f finishLineEnd{ 0.f, 0.f };

    sf::Vector2f forward{ 1.f, 0.f };
    sf::Vector2f right{ 0.f, 1.f };

    std::vector<StartingGridSlot> slots;

    // Debug / visualización.
    bool visible{ true };

    sf::Color finishLineColor{
        255, 255, 255, 255
    };

    sf::Color slotColor{
        255, 200, 0, 220
    };
  };
}
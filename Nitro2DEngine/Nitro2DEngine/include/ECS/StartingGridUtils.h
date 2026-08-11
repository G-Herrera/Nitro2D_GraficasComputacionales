#pragma once

#include "Prerequisites.h"

#include "ECS/Registry.h"
#include "ECS/Components/PathComponent.h"
#include "ECS/Components/StartingGridComponent.h"
#include "Modules/Math2D.h"

#include <algorithm>
#include <cmath>

namespace ECS
{
  inline bool 
  RebuildStartingGrid(Registry& registry, StartingGridComponent& grid)
  {
    grid.slots.clear();

    if (grid.pathEntity == NULL_ENTITY || !registry.IsAlive(grid.pathEntity))
    {
      return false;
    }

    const PathComponent* path = registry.TryGetComponent<PathComponent>(grid.pathEntity);

    if (!path || path->points.size() < 2)
    {
      return false;
    }

    // ----------------------------------------------
    // Anchor index seguro
    // ----------------------------------------------

    const int pointCount = static_cast<int>(path->points.size());

    grid.anchorPointIndex = std::clamp(grid.anchorPointIndex, 0, pointCount - 1);

    const std::size_t anchorIndex = static_cast<std::size_t>(grid.anchorPointIndex);

    std::size_t nextIndex = anchorIndex + 1;

    if (nextIndex >= path->points.size())
    {
      nextIndex = path->closed ? 0 : anchorIndex;
    }

    // ----------------------------------------------
    // Dirección de carrera
    // ----------------------------------------------

    sf::Vector2f forward = Math::Normalize(path->points[nextIndex] - path->points[anchorIndex]);

    if (Math::LengthSquared(forward) <= 0.00001f)
    {
      forward = {1.f, 0.f};
    }

    grid.forward = forward;

    // Perpendicular a la dirección.
    grid.right = {-forward.y, forward.x};

    // ----------------------------------------------
    // Línea de salida/meta
    // ----------------------------------------------

    grid.finishLineCenter = path->points[anchorIndex];

    grid.finishLineStart = grid.finishLineCenter - grid.right * grid.lineHalfWidth;

    grid.finishLineEnd = grid.finishLineCenter + grid.right * grid.lineHalfWidth;

    // ----------------------------------------------
    // Slots
    //
    //     META
    //  ----------
    //
    //     1   2
    //
    //     3   4
    //
    //     5   6
    // ----------------------------------------------

    const int safeSlotCount = std::max(0, grid.slotCount);

    grid.slots.reserve(static_cast<std::size_t>(safeSlotCount));

    const float rotationRadians =std::atan2(forward.y, forward.x);

    constexpr float radiansToDegrees = 180.f / 3.14159265358979323846f;

    const float rotationDegrees = rotationRadians * radiansToDegrees;

    for (int i = 0; i < safeSlotCount; ++i)
    {
      const int row = i / 2;

      const bool leftColumn = (i % 2) == 0;

      const float lateralOffset = leftColumn ? -grid.columnSpacing * 0.5f : grid.columnSpacing * 0.5f;

      const float backwardDistance = grid.firstRowOffset + static_cast<float>(row) * grid.rowSpacing;

      StartingGridSlot slot;

      slot.position = grid.finishLineCenter - grid.forward * backwardDistance + grid.right * lateralOffset;

      slot.rotation = rotationDegrees;

      grid.slots.push_back(slot);
    }

    return true;
  }
}
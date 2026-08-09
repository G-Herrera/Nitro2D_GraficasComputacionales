#pragma once

#include "Prerequisites.h"
#include "ECS/Components/PathComponent.h"
#include "Modules/Math2D.h"

// ======================================================
// ECS :: PathUtils.h
//
// Funciones auxiliares para construir y reconstruir Paths.
//
// PathComponent contiene solamente DATOS.
// Math2D contiene la matemática Catmull-Rom.
// Este helper conecta ambas responsabilidades.
//
// ======================================================

namespace ECS
{
  /**
   * @brief Regenera la polilínea densa de un PathComponent
   * a partir de sus puntos de control.
   *
   * Esta función debe llamarse cada vez que:
   * - se agrega un control point;
   * - se mueve un control point;
   * - se elimina un control point;
   * - cambia samplesPerSegment.
   */
  inline void 
  RebuildPath(PathComponent& path)
  {
    path.points.clear();

    // Catmull-Rom cerrado necesita suficientes puntos
    // para producir una curva útil.
    if (path.controlPoints.size() < 4)
    {
      // Mientras el diseñador todavía está trazando,
      // conservamos una polilínea provisional.
      path.points = path.controlPoints;

      return;
    }

    const int safeSamples = std::max(1,path.samplesPerSegment);

    if (path.closed)
    {
      path.points = Math::BuildClosedCatmullRom(path.controlPoints,safeSamples);

      return;
    }

    // Por ahora el editor avanzado está enfocado
    // en circuitos cerrados.
    //
    // Si closed == false, usamos provisionalmente
    // los control points como polilínea.
    path.points = path.controlPoints;
  }
}
#pragma once
#include "Prerequisites.h"

// ======================================================
// ECS :: Components/PathEditorComponent.h
//
// Estado de EDICION de un Path.
//
// Este componente NO contiene la geometria del camino.
// Los puntos reales viven en PathComponent.
//
// PathEditorSystem utilizara estos datos para saber:
// - si el path esta siendo editado;
// - que herramienta esta activa;
// - que control point esta seleccionado;
// - con que radio se seleccionan puntos.
//
// ======================================================

namespace ECS
{
  enum class 
  PathEditMode
  {
    None = 0,
    Add,
    Move,
    Delete
  };

  struct PathEditorComponent
  {
    // Activa/desactiva la edición interactiva.
    bool enabled{ false };

    // Herramienta actual.
    PathEditMode mode{
        PathEditMode::None
    };

    // Índice del control point seleccionado.
    // -1 significa ninguno.
    int selectedControlPoint{ -1 };

    // Indica si el punto seleccionado está siendo
    // arrastrado actualmente con el mouse.
    bool draggingControlPoint{ false };

    // Radio en unidades de mundo utilizado para
    // seleccionar puntos con el mouse.
    float pointPickRadius{ 18.f };

    // Visualización auxiliar.
    bool showControlPoints{ true };

    // Tamaño visual de cada control point.
    float controlPointDrawRadius{ 7.f };

    PathEditorComponent() = default;
  };
}
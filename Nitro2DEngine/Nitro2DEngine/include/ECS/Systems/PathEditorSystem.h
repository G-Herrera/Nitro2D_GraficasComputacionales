#pragma once

#include "Prerequisites.h"
#include "ECS/System.h"

class Window;

namespace ECS
{
  class Registry;
  struct PathComponent;

  // ======================================================
  // ECS :: PathEditorSystem
  //
  // Gestiona la interacción entre el mouse y los
  // PathComponent editables.
  //
  // Responsabilidades:
  // - Obtener posición del mouse.
  // - Convertir Screen -> World.
  // - Agregar control points.
  // - Seleccionar/mover control points.
  // - Eliminar control points.
  //
  // NO renderiza.
  // NO contiene los datos del Path.
  // ======================================================

  class PathEditorSystem final : public System
  {
  public:

    explicit PathEditorSystem(Window& window);

    ~PathEditorSystem() override = default;

    void 
    OnUpdate(Registry& registry, float deltaTime) override;

    [[nodiscard]]
    const sf::Vector2f&
      GetMouseWorldPosition() const noexcept
    {
      return m_mouseWorldPosition;
    }

  private:

    Window& m_window;

    // Posición del mouse expresada
    // en coordenadas del mundo.
    sf::Vector2f m_mouseWorldPosition{0.f, 0.f};

    // Estado del frame anterior para detectar
    // únicamente el instante inicial del clic.
    bool m_leftMouseWasPressed{ false };

    /**
      * @brief Busca el control point más cercano a una posición
      * dentro de un radio máximo.
      *
      * @return Índice del punto encontrado o -1 si ninguno
      * está suficientemente cerca.
      */
    [[nodiscard]]
    int 
    FindNearestControlPoint(const PathComponent& path,
                            const sf::Vector2f& position,
                            float pickRadius) const noexcept;
  };
}
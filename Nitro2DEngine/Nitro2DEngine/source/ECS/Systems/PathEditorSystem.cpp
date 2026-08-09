#include "ECS/Systems/PathEditorSystem.h"

#include "Core/Window.h"
#include "ECS/Registry.h"
#include "ECS/PathUtils.h"
#include "ECS/Components/PathComponent.h"
#include "ECS/Components/PathEditorComponent.h"

namespace ECS
{
  PathEditorSystem::PathEditorSystem(Window& window): m_window(window){}
  
  int 
  PathEditorSystem::FindNearestControlPoint(const PathComponent& path,
                                            const sf::Vector2f& position,
                                            float pickRadius) const noexcept
  {
    if (path.controlPoints.empty()) return -1;

    const float radiusSq = pickRadius * pickRadius;

    float nearestDistanceSq = radiusSq;

    int nearestIndex = -1;

    for (std::size_t i = 0; i < path.controlPoints.size(); ++i)
    {
      const sf::Vector2f difference = path.controlPoints[i] - position;

      const float distanceSq = difference.x * difference.x + difference.y * difference.y;

      if (distanceSq <= nearestDistanceSq)
      {
        nearestDistanceSq = distanceSq;

        nearestIndex = static_cast<int>(i);
      }
    }

    return nearestIndex;
  }

  void 
  PathEditorSystem::OnUpdate(Registry& registry, float deltaTime)
  {
    (void)deltaTime;

    if (!m_window.m_window)
    {
      return;
    }

    // --------------------------------------------------
    // 1. Obtener posición del mouse en píxeles
    // --------------------------------------------------

    const sf::Vector2i mousePixelPosition =
      sf::Mouse::getPosition(
        *m_window.m_window);

    // --------------------------------------------------
    // 2. Convertir Screen -> World
    // --------------------------------------------------

    m_mouseWorldPosition = m_window.ScreenToWorld(mousePixelPosition);

    // --------------------------------------------------
    // Detectar clic izquierdo solamente una vez
    // --------------------------------------------------

    const bool leftMousePressed = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

    const bool leftMouseClicked = leftMousePressed && !m_leftMouseWasPressed;

    const bool leftMouseReleased = !leftMousePressed && m_leftMouseWasPressed;

    // Si ImGui necesita el mouse, significa que el usuario
    // está interactuando con Inspector, Hierarchy, etc.
    //
    // Ese clic NO debe modificar la escena.
    const bool imguiWantsMouse = ImGui::GetIO().WantCaptureMouse;

    // --------------------------------------------------
    // 3. Buscar Paths actualmente editables
    // --------------------------------------------------

    registry.GetView<PathComponent, PathEditorComponent>().Each(
      [this, leftMouseClicked, leftMousePressed, leftMouseReleased, imguiWantsMouse](
        EntityID entity,
        PathComponent& path,
        PathEditorComponent& editor)
      {
          (void)entity;
          (void)path;

          if (!editor.enabled) return;

          // --------------------------------------------------
          // ADD MODE
          // --------------------------------------------------

          if (editor.mode == PathEditMode::Add)
          {
            if (leftMouseClicked && !imguiWantsMouse)
            {
              path.controlPoints.push_back(m_mouseWorldPosition);

              // Cada modificación de los puntos de control
              // reconstruye inmediatamente la representación
              // runtime del Path.
              RebuildPath(path);

              // El nuevo punto pasa a ser la selección actual.
              editor.selectedControlPoint = static_cast<int>(path.controlPoints.size()) - 1;
            }
          }

          // --------------------------------------
          // MOVE MODE
          // --------------------------------------

          if (editor.mode == PathEditMode::Move)
          {
            // ----------------------------------
            // 1. Intentar seleccionar un punto
            // ----------------------------------

            if (leftMouseClicked && !imguiWantsMouse)
            {
              const int nearestPoint = FindNearestControlPoint(path,
                                                               m_mouseWorldPosition,
                                                               editor.pointPickRadius);

              editor.selectedControlPoint = nearestPoint;

              editor.draggingControlPoint = nearestPoint >= 0;
            }


            // ----------------------------------
            // 2. Arrastrar
            // ----------------------------------

            if (leftMousePressed && editor.draggingControlPoint && editor.selectedControlPoint >= 0)
            {
              const std::size_t index = static_cast<std::size_t>(editor.selectedControlPoint);

              if (index < path.controlPoints.size())
              {
                path.controlPoints[index] = m_mouseWorldPosition;

                RebuildPath(path);
              }
              else
              {
                editor.selectedControlPoint = -1;

                editor.draggingControlPoint = false;
              }
            }


            // ----------------------------------
            // 3. Soltar el punto
            // ----------------------------------

            if (leftMouseReleased)
            {
              editor.draggingControlPoint =
                false;
            }
          }

          // --------------------------------------
          // DELETE MODE
          // --------------------------------------

          if (editor.mode == PathEditMode::Delete)
          {
            if (leftMouseClicked && !imguiWantsMouse)
            {
              const int pointToDelete = FindNearestControlPoint(path,
                                                                m_mouseWorldPosition,
                                                                editor.pointPickRadius);

              if (pointToDelete >= 0)
              {
                const std::size_t index =static_cast<std::size_t>(pointToDelete);

                if (index < path.controlPoints.size())
                {
                  path.controlPoints.erase(path.controlPoints.begin() + static_cast<std::ptrdiff_t>(index));

                  // La selección anterior puede dejar de ser
                  // válida porque los índices cambian después
                  // del erase.
                  editor.selectedControlPoint = -1;

                  editor.draggingControlPoint = false;

                  // Reconstruir inmediatamente la representación
                  // runtime del camino.
                  RebuildPath(path);
                }
              }
            }
          }
          // Solo verificamos que Screen -> World
          // esté funcionando correctamente.
      });

		// Guardar el estado del mouse para el próximo frame.
    m_leftMouseWasPressed = leftMousePressed;
  }
}
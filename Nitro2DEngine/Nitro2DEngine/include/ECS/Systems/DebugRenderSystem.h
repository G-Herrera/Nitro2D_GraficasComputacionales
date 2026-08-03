#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "Core/Window.h"

namespace ECS
{
  class Registry;
  struct PathComponent;
  struct DebugPathComponent;

  class DebugRenderSystem final : public System
  {
  public:
    explicit DebugRenderSystem(Window& window) noexcept
      : m_window(window)
    {
    }

    ~DebugRenderSystem() override = default;

    void OnUpdate(
      Registry& registry,
      float deltaTime) override;

  private:
    void DrawPath(
      const PathComponent& path,
      const DebugPathComponent& debug);

    void DrawCenterLine(
      const PathComponent& path,
      const sf::Color& color);

    void DrawPathBounds(
      const PathComponent& path,
      const sf::Color& color);

    void DrawSamplePoints(
      const PathComponent& path,
      const DebugPathComponent& debug);

    Window& m_window;
  };
}
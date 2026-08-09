#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "Core/Window.h"

namespace ECS
{
  class Registry;
  struct PathComponent;
  struct DebugPathComponent;
  struct Transform;
  struct SteeringDebugComponent;

  class DebugRenderSystem final : public System
  {
  public:
    explicit DebugRenderSystem(Window& window) noexcept : m_window(window){}

    ~DebugRenderSystem() override = default;

    void 
    OnUpdate(Registry& registry, float deltaTime) override;

  private:
    void
    DrawPath(const PathComponent& path, const DebugPathComponent& debug);

    void 
    DrawCenterLine(const PathComponent& path, const sf::Color& color);

    void 
    DrawPathBounds(const PathComponent& path, const sf::Color& color);

    void 
    DrawSamplePoints(const PathComponent& path, const DebugPathComponent& debug);

    void
    DrawAgentDebug(const Transform& transform, const SteeringDebugComponent& debug);

    void 
    DrawVector(const sf::Vector2f& origin, const sf::Vector2f& vector, float scale, const sf::Color& color);

    void 
    DrawLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color);

    void 
    DrawPoint(const sf::Vector2f& position, float radius, const sf::Color& color);

    void 
    DrawControlPoint(const sf::Vector2f& position, float radius, const sf::Color& color);

    Window& m_window;
  };
}
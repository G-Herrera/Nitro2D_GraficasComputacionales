#include "ECS/Systems/DebugRenderSystem.h"
#include "ECS/Registry.h"
#include "ECS/Components/PathComponent.h"
#include "ECS/Components/DebugPathComponent.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/SteeringDebugComponent.h"
#include "Modules/Math2D.h"

namespace ECS
{
  void
    DebugRenderSystem::OnUpdate(Registry& registry, float deltaTime)
  {
    // El sistema no necesita deltaTime para dibujar.
    (void)deltaTime;

    // --------------------------------------------------
    // Debug de paths
    // --------------------------------------------------

    registry.GetView<PathComponent, DebugPathComponent>().Each([this](EntityID entity,
      PathComponent& path,
      DebugPathComponent& debug)
      {
        (void)entity;

        if (!debug.enabled)
        {
          return;
        }

        DrawPath(path, debug);
      });

    // --------------------------------------------------
    // Debug de agentes con Steering
    // --------------------------------------------------

    registry.GetView<Transform, SteeringDebugComponent>().Each([this](EntityID entity,
      Transform& transform,
      SteeringDebugComponent& debug)
      {
        (void)entity;

        if (!debug.enabled)
        {
          return;
        }

        DrawAgentDebug(transform, debug);
      });
  }

  void
    DebugRenderSystem::DrawPath(const PathComponent& path, const DebugPathComponent& debug)
  {
    if (path.points.size() < 2)
    {
      return;
    }

    if (debug.drawPathRadius)
    {
      DrawPathBounds(
        path,
        debug.radiusLineColor);
    }

    if (debug.drawCenterLine)
    {
      DrawCenterLine(
        path,
        debug.centerLineColor);
    }

    if (debug.drawSamplePoints)
    {
      DrawSamplePoints(path, debug);
    }
  }

  void DebugRenderSystem::DrawCenterLine(
    const PathComponent& path,
    const sf::Color& color)
  {
    if (path.points.size() < 2)
    {
      return;
    }

    const std::size_t extraVertex =
      path.closed ? 1u : 0u;

    sf::VertexArray line(
      sf::PrimitiveType::LineStrip,
      path.points.size() + extraVertex);

    for (std::size_t i = 0;
      i < path.points.size();
      ++i)
    {
      line[i].position = path.points[i];
      line[i].color = color;
    }

    if (path.closed)
    {
      line[path.points.size()].position =
        path.points.front();

      line[path.points.size()].color =
        color;
    }

    m_window.draw(line);
  }

  void DebugRenderSystem::DrawPathBounds(
    const PathComponent& path,
    const sf::Color& color)
  {
    if (path.points.size() < 2 ||
      path.radius <= 0.f)
    {
      return;
    }

    const std::size_t pointCount =
      path.points.size();

    const std::size_t extraVertex =
      path.closed ? 1u : 0u;

    sf::VertexArray leftLine(
      sf::PrimitiveType::LineStrip,
      pointCount + extraVertex);

    sf::VertexArray rightLine(
      sf::PrimitiveType::LineStrip,
      pointCount + extraVertex);

    for (std::size_t i = 0;
      i < pointCount;
      ++i)
    {
      const std::size_t previousIndex =
        (i == 0)
        ? (path.closed
          ? pointCount - 1
          : 0)
        : i - 1;

      const std::size_t nextIndex =
        (i + 1 < pointCount)
        ? i + 1
        : (path.closed
          ? 0
          : pointCount - 1);

      const sf::Vector2f tangent =
        Math::Normalize(
          path.points[nextIndex] -
          path.points[previousIndex]);

      sf::Vector2f normal{
          -tangent.y,
          tangent.x
      };

      if (Math::LengthSquared(normal) <= 0.00001f)
      {
        normal = { 0.f, 1.f };
      }

      const sf::Vector2f offset =
        normal * path.radius;

      leftLine[i].position =
        path.points[i] + offset;

      rightLine[i].position =
        path.points[i] - offset;

      leftLine[i].color = color;
      rightLine[i].color = color;
    }

    if (path.closed)
    {
      leftLine[pointCount] =
        leftLine[0];

      rightLine[pointCount] =
        rightLine[0];
    }

    m_window.draw(leftLine);
    m_window.draw(rightLine);
  }

  void
    DebugRenderSystem::DrawSamplePoints(const PathComponent& path, const DebugPathComponent& debug)
  {
    if (path.points.empty())
    {
      return;
    }

    const int safeStep =
      std::max(1, debug.samplePointStep);

    sf::CircleShape marker(3.f);

    marker.setOrigin({ 3.f, 3.f });
    marker.setFillColor(
      debug.samplePointColor);

    for (std::size_t i = 0;
      i < path.points.size();
      i += static_cast<std::size_t>(safeStep))
    {
      marker.setPosition(path.points[i]);
      m_window.draw(marker);
    }
  }

  void
  DebugRenderSystem::DrawLine(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Color& color) 
  {
    sf::VertexArray line(sf::PrimitiveType::Lines, 2);

    line[0].position = start;
    line[0].color = color;

    line[1].position = end;
    line[1].color = color;

    m_window.draw(line);
  }

  void 
  DebugRenderSystem::DrawPoint(const sf::Vector2f& position,float radius,const sf::Color& color)
  {
    if (radius <= 0.f) return;

    sf::CircleShape marker(radius);

    marker.setOrigin({radius, radius});

    marker.setPosition(position);
    marker.setFillColor(color);

    m_window.draw(marker);
  }

  void 
  DebugRenderSystem::DrawVector(const sf::Vector2f& origin, const sf::Vector2f& vector,
                                float scale, const sf::Color& color)
  {
    if (scale <= 0.f) return;

    if (Math::LengthSquared(vector) <= 0.00001f) return;

    const sf::Vector2f end = origin + vector * scale;

    DrawLine(origin, end, color);

    // Pequeño marcador en la punta del vector.
    DrawPoint(end, 2.5f, color);
  }

  void 
  DebugRenderSystem::DrawAgentDebug(const Transform& transform, const SteeringDebugComponent& debug)
  {
    const sf::Vector2f& agentPosition = transform.position;

      

    // --------------------------------------------------
    // Velocidad
    // --------------------------------------------------

    if (debug.drawVelocity) DrawVector(agentPosition, debug.velocity, debug.velocityScale, debug.velocityColor);

    // --------------------------------------------------
    // Posición futura predicha
    // --------------------------------------------------

    if (debug.drawPredictedPosition)
    {
      DrawLine(agentPosition, debug.predictedPosition, debug.predictedPositionColor);

      DrawPoint(debug.predictedPosition, 4.f, debug.predictedPositionColor);
    }

    // --------------------------------------------------
    // Punto más cercano sobre la línea central
    // --------------------------------------------------

    if (debug.drawNearestPathPoint)
    {
      DrawLine(debug.predictedPosition, debug.nearestPathPoint, debug.nearestPathPointColor);

      DrawPoint(debug.nearestPathPoint, 4.f, debug.nearestPathPointColor);
    }

    // --------------------------------------------------
    // Objetivo adelantado sobre el path
    // --------------------------------------------------

    if (debug.drawPathTargetPoint)
    {
      DrawLine(debug.nearestPathPoint, debug.pathTargetPoint, debug.pathTargetPointColor);

      DrawPoint(debug.pathTargetPoint, 5.f, debug.pathTargetPointColor);
    }

    // --------------------------------------------------
    // Fuerza exclusiva de Path Following
    // --------------------------------------------------

    if (debug.drawPathFollowingForce)
    {
      DrawVector(agentPosition, debug.pathFollowingForce, debug.forceScale, debug.pathFollowingForceColor);
    }

    // --------------------------------------------------
    // Fuerza de Separation
    // Por ahora será cero y no se dibujará.
    // --------------------------------------------------

    if (debug.drawSeparationForce)
    {
      DrawVector(agentPosition, debug.separationForce, debug.forceScale, debug.separationForceColor);
    }

    // --------------------------------------------------
    // Fuerza final después de prioridades
    // --------------------------------------------------

    if (debug.drawFinalSteeringForce)
    {
      DrawVector( agentPosition, debug.finalSteeringForce, debug.forceScale, debug.finalSteeringForceColor);
    }
  }
}
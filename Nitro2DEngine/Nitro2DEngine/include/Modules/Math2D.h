#pragma once
#include <SFML/System/Vector2.hpp>

namespace Math
{
  [[nodiscard]]
  float Length(const sf::Vector2f& v) noexcept;

  [[nodiscard]]
  float LengthSquared(const sf::Vector2f& v) noexcept;

  [[nodiscard]]
  sf::Vector2f Normalize(const sf::Vector2f& v) noexcept;

  [[nodiscard]]
  sf::Vector2f Truncate(const sf::Vector2f& v, float maxLength) noexcept;

  [[nodiscard]]
  float Distance(const sf::Vector2f& a,
    const sf::Vector2f& b) noexcept;

  [[nodiscard]]
  float DistanceSquared(const sf::Vector2f& a,
    const sf::Vector2f& b) noexcept;
}
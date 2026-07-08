#include "Prerequisites.h"
#include "Modules/Math2D.h"


namespace Math
{

  float Length(const sf::Vector2f& v) noexcept
  {
    return std::sqrt(v.x * v.x + v.y * v.y);
  }

  float LengthSquared(const sf::Vector2f& v) noexcept
  {
    return v.x * v.x + v.y * v.y;
  }

  sf::Vector2f Normalize(const sf::Vector2f& v) noexcept
  {
    const float len = Length(v);

    if (len <= 0.00001f)
      return {};

    return v / len;
  }

  sf::Vector2f Truncate(const sf::Vector2f& v, float maxLength) noexcept
  {
    const float lenSq = LengthSquared(v);

    if (lenSq <= maxLength * maxLength)
      return v;

    return Normalize(v) * maxLength;
  }

  float Distance(const sf::Vector2f& a,
    const sf::Vector2f& b) noexcept
  {
    return Length(b - a);
  }

  float DistanceSquared(const sf::Vector2f& a,
    const sf::Vector2f& b) noexcept
  {
    return LengthSquared(b - a);
  }

}
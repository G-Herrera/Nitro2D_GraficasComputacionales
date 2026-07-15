#include "Prerequisites.h"
#include "Modules/Math2D.h"

namespace Math
{

  /**
    * @brief Calcula la longitud (magnitud) de un vector 2D.
    */
  float
    Length(const sf::Vector2f& v) noexcept {
    return std::sqrt(v.x * v.x + v.y * v.y);
  }

  /**
    * @brief Calcula el cuadrado de la longitud de un vector 2D.
    */
  float
    LengthSquared(const sf::Vector2f& v) noexcept {
    return v.x * v.x + v.y * v.y;
  }

  /**
    * @brief Normaliza un vector 2D.
    */
  sf::Vector2f
    Normalize(const sf::Vector2f& v) noexcept {
    const float len = Length(v);

    // Evita divisiones entre cero cuando el vector es prácticamente nulo.
    if (len <= 0.00001f)
      return {};

    return v / len;
  }

  /**
    * @brief Limita la longitud de un vector a un valor máximo.
    */
  sf::Vector2f
    Truncate(const sf::Vector2f& v, float maxLength) noexcept {
    const float lenSq = LengthSquared(v);

    // Si el vector ya está dentro del límite, se devuelve sin modificar.
    if (lenSq <= maxLength * maxLength)
      return v;

    // Conserva la dirección original ajustando únicamente su magnitud.
    return Normalize(v) * maxLength;
  }

  /**
    * @brief Calcula la distancia entre dos puntos.
    */
  float
    Distance(const sf::Vector2f& a, const sf::Vector2f& b) noexcept {
    return Length(b - a);
  }

  /**
    * @brief Calcula el cuadrado de la distancia entre dos puntos.
    */
  float
  DistanceSquared(const sf::Vector2f& a, const sf::Vector2f& b) noexcept {
    return LengthSquared(b - a);
  }

}
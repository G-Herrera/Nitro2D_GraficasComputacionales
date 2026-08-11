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

  // ================= Geometria de Path =================

  sf::Vector2f ProjectPointOnSegment(const sf::Vector2f& p,
    const sf::Vector2f& a, const sf::Vector2f& b) noexcept
  {
    const sf::Vector2f ab = b - a;
    const float abLenSq = LengthSquared(ab);

    if (abLenSq <= 0.00001f) return a; // segmento degenerado (a == b)

    float t = ((p.x - a.x) * ab.x + (p.y - a.y) * ab.y) / abLenSq;
    t = std::clamp(t, 0.f, 1.f);

    return a + ab * t;
  }

  NearestPathResult NearestPointOnPath(
    const std::vector<sf::Vector2f>& points, bool closed,
    const sf::Vector2f& p) noexcept
  {
    NearestPathResult result;
    result.distance = std::numeric_limits<float>::max();

    if (points.empty()) return result;

    if (points.size() == 1) {
      result.point = points[0];
      result.distance = Distance(p, points[0]);
      return result;
    }

    const std::size_t segmentCount = closed ? points.size() : points.size() - 1;

    for (std::size_t i = 0; i < segmentCount; ++i) {
      const sf::Vector2f& a = points[i];
      const sf::Vector2f& b = points[(i + 1) % points.size()];

      const sf::Vector2f candidate = ProjectPointOnSegment(p, a, b);
      const float dist = Distance(p, candidate);

      if (dist < result.distance) {
        result.distance = dist;
        result.point = candidate;
        result.segmentIndex = i;
      }
    }

    return result;
  }

  PathProgressResult ComputePathProgress(const std::vector<sf::Vector2f>& points, 
                                         bool closed, const sf::Vector2f& position) noexcept
  {
    PathProgressResult result;

    if (points.size() < 2)
    {
      return result;
    }

    // --------------------------------------------------
    // 1. Encontrar dónde está el agente respecto al Path
    // --------------------------------------------------

    const NearestPathResult nearest = NearestPointOnPath(points, closed, position);

    result.nearestPoint = nearest.point;

    result.segmentIndex = nearest.segmentIndex;


    // --------------------------------------------------
    // 2. Calcular longitud total del Path
    // --------------------------------------------------

    const std::size_t segmentCount = closed ? points.size() : points.size() - 1;

    float totalLength = 0.f;

    for (std::size_t i = 0; i < segmentCount; ++i)
    {
      const std::size_t next = (i + 1) % points.size();

      totalLength += Distance( points[i], points[next]);
    }

    result.totalPathLength = totalLength;

    if (totalLength <= 0.00001f)
    {
      return result;
    }


    // --------------------------------------------------
    // 3. Sumar segmentos ANTERIORES al actual
    // --------------------------------------------------

    float distanceAlongPath = 0.f;

    for (std::size_t i = 0; i < nearest.segmentIndex; ++i)
    {
      const std::size_t next = (i + 1) % points.size();

      distanceAlongPath += Distance( points[i], points[next]);
    }


    // --------------------------------------------------
    // 4. Añadir la distancia dentro del segmento actual
    // --------------------------------------------------

    const sf::Vector2f& segmentStart = points[nearest.segmentIndex];

    distanceAlongPath += Distance( segmentStart, nearest.point);


    result.distanceAlongPath = distanceAlongPath;

    result.normalizedProgress = std::clamp( distanceAlongPath / totalLength, 0.f, 1.f);

    return result;
  }

  sf::Vector2f PointAheadOnPath(
    const std::vector<sf::Vector2f>& points, bool closed,
    std::size_t fromSegmentIndex, const sf::Vector2f& fromPoint,
    float distanceAhead) noexcept
  {
    if (points.empty()) return fromPoint;

    const std::size_t pointCount = points.size();
    const std::size_t segmentCount = closed ? pointCount : pointCount - 1;
    if (segmentCount == 0) return points[0];

    sf::Vector2f current = fromPoint;
    std::size_t segIndex = fromSegmentIndex % segmentCount;
    float remaining = distanceAhead;

    // Recorre segmento por segmento consumiendo `remaining` hasta
    // encontrar donde cae el punto objetivo. Como maximo una vuelta
    // completa al path (segmentCount+1 iteraciones) para evitar
    // bucles infinitos si distanceAhead es mayor que el path entero.
    for (std::size_t iterations = 0; iterations < segmentCount + 1; ++iterations) {
      const sf::Vector2f segEnd = points[(segIndex + 1) % pointCount];
      const float segLen = Distance(current, segEnd);

      if (remaining <= segLen) {
        const sf::Vector2f dir = (segLen > 0.00001f)
          ? (segEnd - current) / segLen
          : sf::Vector2f{ 0.f, 0.f };
        return current + dir * remaining;
      }

      remaining -= segLen;
      current = segEnd;
      segIndex = (segIndex + 1) % segmentCount;

      if (!closed && segIndex == 0) {
        return points.back(); // path abierto: no hay mas camino, quedamos al final
      }
    }

    return current;
  }

  std::vector<sf::Vector2f> BuildClosedCatmullRom(
    const std::vector<sf::Vector2f>& controlPoints,
    int samplesPerSegment) noexcept
  {
    std::vector<sf::Vector2f> result;
    const std::size_t n = controlPoints.size();

    if (n < 3 || samplesPerSegment < 1) return controlPoints;

    const float step = 1.f / static_cast<float>(samplesPerSegment);

    for (std::size_t i = 0; i < n; ++i) {
      const sf::Vector2f& p0 = controlPoints[(i + n - 1) % n];
      const sf::Vector2f& p1 = controlPoints[i];
      const sf::Vector2f& p2 = controlPoints[(i + 1) % n];
      const sf::Vector2f& p3 = controlPoints[(i + 2) % n];

      for (int s = 0; s < samplesPerSegment; ++s) {
        const float t = step * static_cast<float>(s);
        const float t2 = t * t;
        const float t3 = t2 * t;

        const sf::Vector2f point =
          0.5f * ((2.f * p1) +
            (-p0 + p2) * t +
            (2.f * p0 - 5.f * p1 + 4.f * p2 - p3) * t2 +
            (-p0 + 3.f * p1 - 3.f * p2 + p3) * t3);

        result.push_back(point);
      }
    }

    return result;
  }
}
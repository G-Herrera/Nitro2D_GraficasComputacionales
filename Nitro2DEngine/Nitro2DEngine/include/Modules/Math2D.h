#pragma once
#include <SFML/System/Vector2.hpp>

/**
	* @file Math2D.h
  * 
	* @brief Funciones matemáticas 2D para operaciones con vectores.
  * 
	* @details Este archivo contiene funciones matemáticas para realizar operaciones comunes con vectores 2D, 
  * como calcular la longitud, normalizar, truncar y calcular distancias entre vectores. 
  * Estas funciones son útiles en el desarrollo de juegos y simulaciones que requieren manipulación 
  * de vectores en un espacio bidimensional.
  * 
	* @note Todas las funciones están marcadas como [[nodiscard]] para indicar que el valor de retorno no debe ser ignorado,
  */

namespace Math
{
  /**
    * @brief Calcula la longitud (magnitud) de un vector 2D.
    *
    * @param v El vector 2D del cual se desea calcular la longitud.
    * @return La longitud del vector 2D.
    *
    * @note Esta función utiliza la fórmula de la raíz cuadrada de la suma de los cuadrados de las componentes del vector.
    */
  [[nodiscard]]
  float Length(const sf::Vector2f& v) noexcept;

  /**
    * @brief Calcula el cuadrado de la longitud (magnitud) de un vector 2D.
    *
    * @param v El vector 2D del cual se desea calcular el cuadrado de la longitud.
    * @return El cuadrado de la longitud del vector.
    *
    * @note Esta función evita el cálculo de la raíz cuadrada, por lo que es más eficiente
    * cuando únicamente se requiere comparar magnitudes entre vectores.
    */
  [[nodiscard]]
  float LengthSquared(const sf::Vector2f& v) noexcept;

  /**
    * @brief Normaliza un vector 2D.
    *
    * @param v El vector 2D que se desea normalizar.
    * @return Un vector unitario con la misma dirección que el vector original.
    *
    * @note Si el vector tiene longitud cero, la función devuelve un vector nulo
    * para evitar una división entre cero.
    */
  [[nodiscard]] sf::Vector2f 
  Normalize(const sf::Vector2f& v) noexcept;

  /**
    * @brief Limita la longitud de un vector a un valor máximo.
    *
    * @param v El vector 2D que se desea limitar.
    * @param maxLength Longitud máxima permitida para el vector.
    * @return Un vector cuya longitud no supera el valor especificado.
    *
    * @note Si la longitud del vector es menor o igual a @p maxLength, se devuelve
    * el vector original sin modificaciones.
    */
  [[nodiscard]] sf::Vector2f 
  Truncate(const sf::Vector2f& v, float maxLength) noexcept;

  /**
    * @brief Calcula la distancia entre dos puntos en un espacio 2D.
    *
    * @param a Primer punto.
    * @param b Segundo punto.
    * @return La distancia entre ambos puntos.
    *
    * @note Esta función utiliza la distancia euclidiana, calculando la raíz cuadrada
    * de la suma de los cuadrados de las diferencias entre sus componentes.
    */
  [[nodiscard]] float 
  Distance(const sf::Vector2f& a, const sf::Vector2f& b) noexcept;

  /**
    * @brief Calcula el cuadrado de la distancia entre dos puntos en un espacio 2D.
    *
    * @param a Primer punto.
    * @param b Segundo punto.
    * @return El cuadrado de la distancia entre ambos puntos.
    *
    * @note Esta función evita el cálculo de la raíz cuadrada, siendo más eficiente
    * cuando únicamente se requiere comparar distancias.
    */
  [[nodiscard]] float 
  DistanceSquared(const sf::Vector2f& a, const sf::Vector2f& b) noexcept;

  // ================= Geometria de Path =================

  // Proyecta el punto p sobre el segmento [a,b]; devuelve el punto
  // mas cercano DENTRO del segmento (clamp del parametro t en [0,1]).
  [[nodiscard]]
  sf::Vector2f ProjectPointOnSegment(const sf::Vector2f& p, const sf::Vector2f& a, 
                                     const sf::Vector2f& b) noexcept;

  // Resultado de buscar el punto mas cercano sobre una polilinea completa.
  struct NearestPathResult {
    sf::Vector2f point;              // punto mas cercano sobre el path
    std::size_t segmentIndex{ 0 };   // indice del segmento [i, i+1] donde cae
    float distance{ 0.f };           // distancia de p a ese punto
  };

  // Resultado de medir dónde se encuentra un punto
// respecto al recorrido completo de un Path.
  struct PathProgressResult
  {
    // Punto proyectado sobre la línea central.
    sf::Vector2f nearestPoint{
        0.f,
        0.f
    };

    // Segmento donde cayó la proyección.
    std::size_t segmentIndex{
        0
    };

    // Distancia recorrida desde points[0]
    // hasta nearestPoint siguiendo la polilínea.
    float distanceAlongPath{
        0.f
    };

    // Longitud total de la polilínea.
    float totalPathLength{
        0.f
    };

    // Progreso normalizado:
    // 0.0 = salida/meta
    // 0.5 = mitad de vuelta
    // 1.0 = casi de regreso a meta
    float normalizedProgress{
        0.f
    };
  };

  /**
    * @brief Calcula cuánto ha avanzado un punto a lo largo
    * de una polilínea.
    *
    * El progreso se mide siguiendo los segmentos del path,
    * no mediante distancia euclidiana.
    */
  [[nodiscard]]
  PathProgressResult ComputePathProgress(const std::vector<sf::Vector2f>& points,
                                         bool closed, const sf::Vector2f& position) noexcept;

  // Recorre todos los segmentos de la polilinea (points) y devuelve el
  // punto mas cercano a p. Si closed=true, incluye el segmento que
  // conecta el ultimo punto con el primero.
  [[nodiscard]]
  NearestPathResult NearestPointOnPath(const std::vector<sf::Vector2f>& points, bool closed, 
                                       const sf::Vector2f& p) noexcept;

  // Partiendo de un punto ya ubicado sobre el path (fromSegmentIndex,
  // fromPoint), avanza `distanceAhead` unidades SIGUIENDO los segmentos
  // de la polilinea y devuelve el punto resultante. Si closed=true, al
  // llegar al final vuelve a empezar por el principio (vuelta cerrada).
  [[nodiscard]]
  sf::Vector2f PointAheadOnPath(const std::vector<sf::Vector2f>& points, bool closed,
                                std::size_t fromSegmentIndex, const sf::Vector2f& fromPoint,
                                float distanceAhead) noexcept;

  // Genera una polilinea DENSA y cerrada interpolando pocos puntos de
  // control con Catmull-Rom uniforme. Uso previsto: construir el path
  // del circuito UNA sola vez al crear la escena, muestreando varios
  // puntos por segmento para que se vea suave en runtime sin tener que
  // evaluar la curva cada frame.
  [[nodiscard]]
  std::vector<sf::Vector2f> BuildClosedCatmullRom(const std::vector<sf::Vector2f>& controlPoints,
                                                  int samplesPerSegment) noexcept;
};
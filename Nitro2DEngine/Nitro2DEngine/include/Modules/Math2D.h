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
};
#pragma once
#include "Prerequisites.h"
class Window;

/**
	* @brief Clase CShape que representa una forma gráfica en el motor Nitro 2D Engine.
	* 
	* @details La clase CShape es una envoltura para las formas gráficas proporcionadas por la biblioteca SFML.
	*/
class CShape
{
public:
	CShape() = default;

	/**
		* @brief Constructor de la clase CShape que recibe un tipo de forma como parámetro.
		* @param shapeType El tipo de forma que se desea crear (CIRCLE, RECTANGLE, TRIANGLE, POLYGON, LINE).
		*/
	explicit CShape(ShapeType shapeType);

	~CShape() = default;

	/**
		* @brief Método para dibujar la forma en la ventana.
		* @param window La ventana donde se desea dibujar la forma.
		*/
	void 
	draw(Window& window);

	/**
		* @brief Método para obtener un puntero a la forma gráfica.
		* @return Un puntero a la forma gráfica de tipo sf::Shape.
		*/
	sf::Shape* 
	getShape();

private:
	static std::unique_ptr<sf::Shape> createShape(ShapeType shapeType); /// Método estático para crear una forma gráfica basada en el tipo de forma proporcionado.

private:
	std::unique_ptr<sf::Shape> m_shape = nullptr; /// Puntero único a la forma gráfica de tipo sf::Shape.
	ShapeType m_shapeType; /// Variable para almacenar el tipo de forma que se ha creado.
};

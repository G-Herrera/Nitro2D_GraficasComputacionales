#include "Core/CShape.h"
#include "Core/Window.h"

/**
	* @brief Implementación de la clase CShape que representa una forma gráfica en el motor Nitro 2D Engine.
	* @details La clase CShape es una envoltura para las formas gráficas proporcionadas por la biblioteca SFML.
	*/

/**
	* @brief Constructor de la clase CShape que recibe un tipo de forma como parámetro.
	* @param shapeType El tipo de forma que se desea crear (CIRCLE, RECTANGLE, TRIANGLE, POLYGON, LINE).
	*/
CShape::CShape(ShapeType shapeType) : m_shapeType(shapeType), m_shape(createShape(shapeType)) {

}

/**
	* @brief Método para dibujar la forma en la ventana.
	* @param window La ventana donde se desea dibujar la forma.
	*/ 
void 
CShape::draw(Window& window) {
	if (m_shape) {
		window.draw(*m_shape);
	}
}

/**
 * @brief Obtiene un puntero a la figura gráfica asociada al objeto.
 *
 * Devuelve el puntero gestionado internamente por `m_shape` sin transferir
 * la propiedad del recurso.
 *
 * @return Puntero a la instancia de `sf::Shape` almacenada, o `nullptr`
 *         si no existe ninguna figura asociada.
 */
sf::Shape* 
CShape::getShape() {
	return m_shape.get();
}

/**
 * @brief Crea una figura SFML según el tipo especificado.
 *
 * Genera dinámicamente una figura de SFML configurada con valores
 * predeterminados de tamaño, color y posición. La propiedad del objeto
 * creado se devuelve mediante un `std::unique_ptr`.
 *
 * Tipos soportados:
 * - EMPTY: no crea ninguna figura.
 * - CIRCLE: crea un círculo.
 * - RECTANGLE: crea un rectángulo.
 * - TRIANGLE: crea un triángulo mediante `sf::ConvexShape`.
 * - POLYGON: crea un polígono de cinco lados mediante `sf::ConvexShape`.
 * - LINE: crea una línea representada como un rectángulo delgado.
 *
 * @param shapeType Tipo de figura a crear.
 * @return `std::unique_ptr<sf::Shape>` que contiene la figura creada.
 *         Devuelve `nullptr` si el tipo es `EMPTY` o no es válido.
 */
std::unique_ptr<sf::Shape> CShape::createShape(ShapeType shapeType) {
	switch (shapeType) {
		case EMPTY:
			return nullptr;
		case CIRCLE:
		{
			auto circle = std::make_unique<sf::CircleShape>(50.f);
			circle->setFillColor(sf::Color(100, 250, 50));
			circle->setPosition({ 100.f, 100.f });
			return circle;
		}
		case RECTANGLE:
		{
			auto rectangle = std::make_unique<sf::RectangleShape>(sf::Vector2f(120.f, 60.f));
			rectangle->setFillColor(sf::Color(250, 100, 50));
			rectangle->setPosition({ 200.f, 200.f });
			return rectangle;
		}
		case TRIANGLE:
		{
			auto triangle = std::make_unique<sf::ConvexShape>(3);
			triangle->setPoint(0, sf::Vector2f(0.f, 0.f));
			triangle->setPoint(1, sf::Vector2f(100.f, 0.f));
			triangle->setPoint(2, sf::Vector2f(50.f, 100.f));
			triangle->setFillColor(sf::Color(250, 250, 50));
			triangle->setPosition({ 300.f, 300.f });
			return triangle;
		}
		case POLYGON:
		{
			auto polygon = std::make_unique<sf::ConvexShape>(5);
			polygon->setPoint(0, sf::Vector2f(0.f, 0.f));
			polygon->setPoint(1, sf::Vector2f(100.f, 0.f));
			polygon->setPoint(2, sf::Vector2f(120.f, 50.f));
			polygon->setPoint(3, sf::Vector2f(60.f, 100.f));
			polygon->setPoint(4, sf::Vector2f(0.f, 50.f));
			polygon->setFillColor(sf::Color(250, 50, 250));
			polygon->setPosition({ 400.f, 400.f });
			return polygon;
		}
		case LINE:
		{
			auto line = std::make_unique<sf::RectangleShape>(sf::Vector2f(200.f, 5.f));
			line->setFillColor(sf::Color(50, 250, 250));
			line->setPosition({ 500.f, 500.f });
			return line;
		}
		default:
			return nullptr;
			break;
	}
	return std::unique_ptr<sf::Shape>();

}

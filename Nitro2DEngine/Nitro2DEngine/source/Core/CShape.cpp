#include "Core/CShape.h"
#include "Core/Window.h"

CShape::CShape(ShapeType shapeType) : m_shapeType(shapeType), m_shape(createShape(shapeType)) {

}

void 
CShape::draw(Window& window) {
	if (m_shape) {
		window.draw(*m_shape);
	}
}

sf::Shape* 
CShape::getShape() {
	return m_shape.get();
}

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

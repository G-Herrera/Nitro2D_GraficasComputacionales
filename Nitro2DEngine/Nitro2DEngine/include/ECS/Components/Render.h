#pragma once
#include "Prerequisites.h"

// ================================================================
// ECS :: Components/Render.h
// 
// Render - componente de DATOS PUROS que guarda la forma
// dibujable (sf::shape) de una entidad.
// 
// Se almacena en un ComponentPool<Render>. Usa shared_ptr
// (movible y copiable) para ser compatible con el pool,
// a diferencia de CShape, que tiene el move suprimido.
// 
// La posición/rotación/escala NO viven aquí: vienen del
// componenete Transform y las aplica el RenderSystem cada frame.
// ================================================================

namespace ECS {

	struct Render {
		std::shared_ptr<sf::Shape> shape;					// la forma a dibujar
		std::shared_ptr<sf::Texture> texture;     // sprite opciona (nullptr sin sprite)
		sf::Color fillColor{ sf::Color::White };  // color de relleno
		bool visible{ true };											// permite ocultar si nquitar el componente

		Render() = default;

		explicit Render(std::shared_ptr<sf::Shape> s, sf::Color color = sf::Color::White) 
										noexcept : shape(std::move(s)), fillColor(color){}

		/*
		 * @brief Establece la textura de la forma a partir de un archivo.
		 * 
		 * @param path La ruta del archivo de textura.
		 * @param resetRect Si es true, se reinicia el rectángulo de textura de la forma.
		 * 
		 * @return true si la textura se cargó correctamente, false en caso contrario.
		 */
		bool 
		SetTexture(const std::string& path, bool resetRect = true) 
		{
			if (!shape) return false;
			auto tex = std::make_shared<sf::Texture>();
			if (!tex->loadFromFile(path)) return false;
			texture = std::move(tex);
			shape->setTexture(texture.get(), resetRect);
			return true;
		}

		/**
			* @brief Establece la textura de la forma a partir de un objeto sf::Texture.
			* 
			* @param tex Un puntero compartido a un objeto sf::Texture.
			* @param resetRect Si es true, se reinicia el rectángulo de textura de la forma.
			*/
		void 
		SetTexture(std::shared_ptr<sf::Texture> tex, bool resetRect = true) 
		{
			if (!shape) return;
			texture = std::move(tex);
			shape->setTexture(texture ? texture.get() : nullptr, resetRect);
		}

		/**
			* @brief Elimina la textura de la forma y libera la memoria asociada.
			*/
		void 
		ClearTexture() 
		{
			if (!shape) return;
			texture.reset();
			shape->setTexture(nullptr);
		}

		/**
			* @brief Crea un componente Render con una forma predefinida.
			* 
			* @param type El tipo de forma a crear (CIRCLE, RECTANGLE, TRIANGLE, POLYGON, LINE).
			* @param color El color de relleno de la forma (por defecto blanco).
			* @param texturePath La ruta del archivo de textura (opcional).
			* 
			* @return Un objeto Render con la forma especificada.
			*/
		[[nodiscard]] static Render
		Make(ShapeType type, 
				 sf::Color color = sf::Color::White, 
				 const std::string& texturePath = "") 
		{
			std::shared_ptr<sf::Shape> s;

			/**
				* @brief Crea una forma de acuerdo al tipo especificado.
				*/
			switch (type)
			{
			/**
				* @brief Crea una forma vacía (sin forma).
				*/
			case EMPTY:
				break;
			/**
				* @brief Crea un círculo con radio 50 y origen en el centro.
				*/
			case CIRCLE: {
				auto c = std::make_shared<sf::CircleShape>(50.f);
				c->setOrigin({ 50.f, 50.f });
				s = c;
				break;
			}
			/**
				* @brief Crea un rectángulo con tamaño 100x50 y origen en el centro.
				*/
			case RECTANGLE: {
				sf::Vector2f size{ 100.f, 50.f };
				auto r = std::make_shared<sf::RectangleShape>(size);
				r->setOrigin(size / 2.f);
				s = r;
				break;
			}
			/**
				* @brief Crea un triángulo equilátero con vértices en (0,0), (100,0) y (50,100), y origen en el centro.
				*/
			case TRIANGLE: {
				auto t = std::make_shared<sf::ConvexShape>(3);
				t->setPoint(0, { 0.f, 0.f });
				t->setPoint(1, { 100.f, 0.f });
				t->setPoint(2, { 50.f, 100.f });
				t->setOrigin({ 50.f, 50.f });
				s = t;
				break;
			}
			/**
				* @brief Crea un polígono con 5 vértices y origen en (0,0).
				*/
			case POLYGON: {
				auto p = std::make_shared<sf::ConvexShape>(5);
				p->setPoint(0, { 0.f, 0.f });
				p->setPoint(1, { 100.f, 0.f });
				p->setPoint(2, { 120.f, 50.f });
				p->setPoint(3, { 60.f, 100.f });
				p->setPoint(4, { 0.f, 50.f });
				p->setOrigin({ 0.f,0.f });
				s = p;
				break;
			}
			/**
				* @brief Crea una línea representada como un rectángulo delgado con tamaño 200x5 y origen en (0,0).
				*/
			case LINE: {
				auto l = std::make_shared<sf::RectangleShape>(sf::Vector2f(200.f, 5.f));
				l->setFillColor(sf::Color(50, 250, 250));
				l->setPosition({ 500.f, 500.f });
				s = l;
				break;
			}
			default:
				break;
			}

			if (s) s->setFillColor(color);

			Render render{ s, color };
			if (!texturePath.empty()) render.SetTexture(texturePath);
			return render;
		}
	};
}
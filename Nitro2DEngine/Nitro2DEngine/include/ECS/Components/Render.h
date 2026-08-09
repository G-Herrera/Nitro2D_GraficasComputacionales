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
		ShapeType shapeType{ RECTANGLE };											// Tipo lógico de forma utilizado por el Inspector.
		std::shared_ptr<sf::Shape> shape;											// Forma concreta que SFML dibuja.
		std::shared_ptr<sf::Texture> texture;									// Textura cargada actualmente.																											
		std::string texturePath;															// Ruta utilizada para cargar la textura.
																													// Se conserva para mostrarla y recargarla desde el Inspector.
		sf::Color fillColor{ sf::Color::White };							// Propiedades visuales generales.
		bool visible{ true };																	// permite ocultar si nquitar el componente
		int zOrder{ 0 };																			// orden de dibujo: menor = mas atras, mayor = mas al frente

		Render() = default;

		explicit Render(ShapeType type, std::shared_ptr<sf::Shape> s, sf::Color color = sf::Color::White) 
										noexcept : shapeType(type), shape(std::move(s)), fillColor(color){}

		/*
		 * @brief Establece la textura de la forma a partir de un archivo.
		 * 
		 * @param path La ruta del archivo de textura.
		 * @param resetRect Si es true, se reinicia el rectángulo de textura de la forma.
		 * 
		 * @return true si la textura se cargó correctamente, false en caso contrario.
		 */
		bool SetTexture(const std::string& path,bool resetRect = true)
		{
			if (!shape || path.empty()) return false;
			auto newTexture =std::make_shared<sf::Texture>();
			if (!newTexture->loadFromFile(path)) return false;
			texture = std::move(newTexture);
			texturePath = path;
			shape->setTexture(texture.get(),resetRect);

			return true;
		}

		/**
			* @brief Establece la textura de la forma a partir de un objeto sf::Texture.
			* 
			* @param tex Un puntero compartido a un objeto sf::Texture.
			* @param resetRect Si es true, se reinicia el rectángulo de textura de la forma.
			*/
		void 
		SetTexture(std::shared_ptr<sf::Texture> newTexture, bool resetRect = true)
		{
			if (!shape) return;

			texture = std::move(newTexture);

			texturePath.clear();

			shape->setTexture(texture ? texture.get() : nullptr, resetRect);
		}

		/**
			* @brief Elimina la textura de la forma y libera la memoria asociada.
			*/
		void 
		ClearTexture()
		{
			texture.reset();
			texturePath.clear();

			if (shape) shape->setTexture(nullptr);
		}

		/**
			* @brief Crea la forma SFML correspondiente a un ShapeType.
			*
			* Esta función centraliza la geometría por defecto de cada forma.
			* Se utiliza tanto al crear el componente como al cambiar el tipo
			* desde el Inspector.
			*/
		[[nodiscard]] static std::shared_ptr<sf::Shape>
		CreateShape(ShapeType type)
		{
			switch (type)
			{
			case EMPTY:
				return nullptr;

			case CIRCLE:
			{
				auto circle = std::make_shared<sf::CircleShape>(50.f);
				circle->setOrigin({50.f, 50.f});
				return circle;
			}

			case RECTANGLE:
			{
				const sf::Vector2f size{100.f,50.f};
				auto rectangle = std::make_shared<sf::RectangleShape>(size);
				rectangle->setOrigin(size / 2.f);
				return rectangle;
			}

			case TRIANGLE:
			{
				auto triangle =std::make_shared<sf::ConvexShape>(3);
				triangle->setPoint(0,{ 0.f, 0.f });
				triangle->setPoint(1,{ 100.f, 0.f });
				triangle->setPoint(2,{ 50.f, 100.f });
				triangle->setOrigin({50.f,50.f});

				return triangle;
			}

			case POLYGON:
			{
				auto polygon =std::make_shared<sf::ConvexShape>(5);

				polygon->setPoint(0,{ 0.f, 0.f });
				polygon->setPoint(1,{ 100.f, 0.f });
				polygon->setPoint(2,{ 120.f, 50.f });
				polygon->setPoint(3,{ 60.f, 100.f });
				polygon->setPoint(4,{ 0.f, 50.f });
				polygon->setOrigin({60.f,50.f});

				return polygon;
			}

			case LINE:
			{
				const sf::Vector2f size{200.f,5.f};
				auto line = std::make_shared<sf::RectangleShape>(size);
				line->setOrigin({0.f,size.y / 2.f});

				return line;
			}

			default:
				return nullptr;
			}
		}

		/**
			* @brief Sustituye la geometría por otro ShapeType.
			*
			* Conserva las propiedades generales del componente:
			* color, textura, visibilidad y zOrder.
			*
			* @param newType Nuevo tipo de forma.
			*/
		void 
		RebuildShape(ShapeType newType)
		{
			shapeType = newType;

			shape = CreateShape(newType);

			if (!shape) return;

			shape->setFillColor(fillColor);

			if (texture) shape->setTexture(texture.get(), true);
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
		Make(ShapeType type, sf::Color color = sf::Color::White, const std::string& texturePath = "")
		{
			Render render{type,CreateShape(type),color};
			if (render.shape) render.shape->setFillColor(color);
			if (!texturePath.empty()) render.SetTexture(texturePath);

			return render;
		}
	};
}
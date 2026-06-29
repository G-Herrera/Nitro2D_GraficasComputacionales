#pragma once
#include "Prerequisites.h"

/**
 * @class Window
 * @brief Encapsula una ventana de renderizado basada en SFML.
 *
 * Proporciona una interfaz simplificada para la creación, gestión de
 * eventos, renderizado y control del ciclo de vida de una ventana gráfica.
 */
class 
Window {
public:
	Window() = default;

	/**
		* @brief Constructor de la clase Window que recibe el ancho, alto y título de la ventana como parámetros.
		* @param width El ancho de la ventana en píxeles.
		* @param height El alto de la ventana en píxeles.
		* @param title El título de la ventana.
		*/
	Window(int  width, int height, const std::string& title);
	~Window();

	//void 
	//handleEvents(EngineGUI& engineGUI);

	/**
		*	@brief Método para verificar si la ventana está abierta.
		* @return Devuelve true si la ventana está abierta, de lo contrario devuelve false.
		*/
	bool
	isOpen() const;

	/**
		* @brief Método para limpiar la ventana con un color específico.
		* @param color El color con el que se desea limpiar la ventana. Por defecto, se utiliza el color azul (0, 0, 255).
		*/
	void
	clear(const sf::Color& color = sf::Color(0, 0, 255));

	/**
		* @brief Método para dibujar un objeto gráfico en la ventana.
		* @param drawable El objeto gráfico que se desea dibujar, que debe ser una instancia de una clase que herede de sf::Drawable.
		* @param state Las opciones de renderizado que se desean aplicar al dibujar el objeto gráfico. Por defecto, se utilizan las opciones de renderizado predeterminadas (sf::RenderStates::Default).
		*/
	void
	draw(const sf::Drawable& drawable,
			 const sf::RenderStates& state = sf::RenderStates::Default);

	/**
		* @brief Método para mostrar el contenido de la ventana en la pantalla.
		*/
	void
	display();

	/**
		* @brief Método para cerrar la ventana.
		*/
	void
	close();

	void 
	applyCameraView(const sf::Vector2f& center, float zoom, float rotationDeg);

	void
	handleResize(const sf::Vector2u& size);

	/**
		* @brief Método para actualizar el estado de la ventana
		*/ 
	void
	update();

	/**
		* @brief Método para renderizar el contenido de la ventana.
		*/
	void
	render();

	/**
		* @brief Método para destruir la ventana y liberar los recursos asociados.
		*/
	void
	destroy();

	/**
		* @brief Método para obtener el tiempo transcurrido desde la última actualización de la ventana.
		* @return Devuelve un objeto sf::Time que representa el tiempo transcurrido desde la última actualización de la ventana.
		*/
	std::optional<sf::Event>
	pollEvent();

	std::unique_ptr<sf::RenderWindow> m_window = nullptr; /// Puntero único a la ventana gráfica de tipo sf::RenderWindow, que se utiliza para manejar la ventana y sus eventos.
private:
	sf::View m_view; /// Objeto de tipo sf::View que representa la vista de la ventana, utilizada para controlar la visualización del contenido en la ventana.
	sf::Time m_deltaTime; /// Objeto de tipo sf::Time que representa el tiempo transcurrido entre actualizaciones de la ventana, utilizado para controlar la velocidad de actualización y renderizado del contenido en la ventana.
	sf::Clock m_clock; /// Objeto de tipo sf::Clock que se utiliza para medir el tiempo transcurrido desde la última actualización de la ventana, utilizado para calcular el tiempo delta entre actualizaciones.
	sf::Vector2f m_baseViewSize{};

};
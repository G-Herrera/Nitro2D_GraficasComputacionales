#include "Core/Window.h"

/**
 * @brief Construye una ventana de renderizado.
 *
 * Crea una instancia de `sf::RenderWindow` con las dimensiones y título
 * especificados. Si la creación es exitosa, establece un límite de
 * 60 FPS para la ventana.
 *
 * @param width Ancho de la ventana en píxeles.
 * @param height Alto de la ventana en píxeles.
 * @param title Título que se mostrará en la barra de la ventana.
 */
Window::Window(int width, int height, const std::string& title) {
	m_window = std::make_unique<sf::RenderWindow>(sf::VideoMode({ static_cast<unsigned int>(width),
																	static_cast<unsigned int>(height) }), 
																	title, sf::Style::Default);

	if (m_window) {
		m_window->setFramerateLimit(60);
		MESSAGE("Window", "Window", "Window created successfully");
	}
	else
	{
		ERROR("Window", "Window", "Failed to create window");
	}
}

/**
 * @brief Destructor por defecto.
 *
 * Libera automáticamente los recursos asociados a la ventana.
 */
Window::~Window() = default;

/**
 * @brief Comprueba si la ventana permanece abierta.
 *
 * @return true si la ventana existe y está abierta.
 * @return false si la ventana está cerrada o no existe.
 */
bool
Window::isOpen() const {
	if (m_window)
	{
		return m_window && m_window->isOpen();
	}
	else
	{
		ERROR("Window", "isOpen", "Widnow is null");
		return false;
	}

}

/**
 * @brief Limpia el contenido actual de la ventana.
 *
 * Rellena toda la superficie de renderizado con el color indicado.
 *
 * @param color Color utilizado para limpiar la ventana.
 */
void
Window::clear(const sf::Color& color) {
	if (m_window) {
		m_window->clear(color);
	}
	else
	{
		ERROR("Window", "clear", "Window is null");
	}
}

/**
 * @brief Cierra la ventana.
 *
 * Marca la ventana para finalizar su ejecución y procesamiento de eventos.
 */
void
Window::close() {
	if (m_window) {
		m_window->close();
	}
	else
	{
		ERROR("Window", "close", "Window is null");
	}
}

/**
 * @brief Dibuja un objeto renderizable en la ventana.
 *
 * Renderiza cualquier objeto derivado de `sf::Drawable` utilizando los
 * estados de renderizado especificados.
 *
 * @param drawable Objeto a dibujar.
 * @param state Estados de renderizado aplicados durante el dibujo.
 */
void
Window::draw(const sf::Drawable& drawable, const sf::RenderStates& state) {
	if (m_window) {
		m_window->draw(drawable, state);
	}
	else
	{
		ERROR("Window", "draw", "Window is null");
	}
}

/**
 * @brief Presenta en pantalla el contenido renderizado.
 *
 * Intercambia los buffers internos de la ventana mostrando los elementos
 * dibujados desde la última llamada a `clear()`.
 */
void
Window::display() {
	if (m_window) {
		m_window->display();
	}
	else
	{
		ERROR("Window", "display", "Window is null");
	}
}

/**
 * @brief Actualiza el estado temporal de la ventana.
 *
 * Reinicia el reloj interno y almacena el tiempo transcurrido desde
 * la última actualización.
 */
void
Window::update() {
	//Save deltatime once
	m_deltaTime = m_clock.restart();
}

/**
 * @brief Ejecuta el proceso de renderizado de la ventana.
 *
 * Método reservado para centralizar la lógica de renderizado de los
 * elementos gráficos asociados a la ventana.
 */
void
Window::render() {
	//Render all the drawables
}

/**
 * @brief Obtiene el siguiente evento pendiente de la ventana.
 *
 * Extrae un evento de la cola interna de eventos de SFML.
 *
 * @return Un objeto `std::optional<sf::Event>` que contiene el evento
 *         recuperado si existe alguno; en caso contrario devuelve
 *         `std::nullopt`.
 */
std::optional<sf::Event>
Window::pollEvent()
{
	if (m_window) 
	{
		return m_window->pollEvent();
	}

	return std::nullopt;
}

/**
 * @brief Destruye explícitamente la ventana.
 *
 * Libera la instancia interna de `sf::RenderWindow` y sus recursos
 * asociados.
 */
void
Window::destroy() {
	m_window.reset();
}
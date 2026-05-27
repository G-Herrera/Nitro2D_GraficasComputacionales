#include "Core/Window.h"

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

Window::~Window() = default;

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

void
Window::update() {
	//Save deltatime once
	m_deltaTime = m_clock.restart();
}

void
Window::render() {
	//Render all the drawables
}

void
Window::destroy() {
	m_window.reset();
}
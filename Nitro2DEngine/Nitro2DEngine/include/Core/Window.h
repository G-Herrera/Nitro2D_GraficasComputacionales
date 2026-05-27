#pragma once
#include "Prerequisites.h"


class 
Window {
public:
	Window() = default;
	Window(int  width, int height, const std::string& title);
	~Window();

	//void 
	//handleEvents(EngineGUI& engineGUI);

	bool
	isOpen() const;

	void
	clear(const sf::Color& color = sf::Color(0, 0, 255));

	void
	draw(const sf::Drawable& drawable,
			 const sf::RenderStates& state = sf::RenderStates::Default);

	void
	display();

	void
	close();

	void
	update();

	void
	render();

	void
	destroy();

	std::optional<sf::Event>
	pollEvent();

	std::unique_ptr<sf::RenderWindow> m_window = nullptr;
private:
	sf::View m_view;
	sf::Time m_deltaTime;
	sf::Clock m_clock;

};
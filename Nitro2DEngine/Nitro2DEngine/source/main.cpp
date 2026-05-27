#include <Prerequisites.h>
#include <Core/Window.h>
#include <Core/CShape.h>


//Window* g_window = nullptr;
CShape Circle(ShapeType::CIRCLE);



int main()
{
  // create the window
	Window window(800, 600, "Nitro 2D Engine");
  //sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Nitro 2D Engine");

	// set the shape's color to green
	Circle.getShape()->setFillColor(sf::Color(100, 250, 50));

  // run the program as long as the window is open
  while (window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    while (const std::optional event = window.pollEvent())
    {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>()) 
      {
        window.close();
      }
    }

    // clear the window with a gray color
    window.clear(sf::Color(122, 122, 122));

    // draw everything here...
    Circle.draw(window);

    // end the current frame
    window.display();
  }
}
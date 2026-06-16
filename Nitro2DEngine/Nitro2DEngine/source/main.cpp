#include <Prerequisites.h>
#include <Core/Window.h>
#include <Core/CShape.h>
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "ECS/Systems/RenderSystem.h"


//Window* g_window = nullptr;
CShape Circle(ShapeType::CIRCLE);

ECS::Registry registry;

Window g_window(800, 600, "Nitro 2D Engine");
/*
* @brief Funcion principal del programa, donde se crea la ventana y se ejecuta el bucle principal del juego.
* @details En esta función se crea una ventana de 800x600 píxeles con el título "Nitro 2D Engine". 
* Luego, se establece el color de la forma a verde y se inicia un bucle que se ejecuta mientras la ventana esté abierta. 
* Dentro del bucle, se manejan los eventos de la ventana, se limpia la pantalla con un color gris, se dibuja la forma y se muestra el contenido en la pantalla.
* 
* @return int Devuelve 0 al finalizar la ejecución del programa.
* 
* @note Es importante destacar que esta función es el punto de entrada del programa y es donde se inicializan los recursos necesarios para el funcionamiento del motor gráfico.
*/


int main()
{
  registry.AddSystem<ECS::RenderSystem>(g_window);

  ECS::EntityID circle = registry.CreateEntity();
  registry.AddComponent<ECS::Transform>(circle, sf::Vector2f{ 400.f, 300.f });
  registry.AddComponent<ECS::Render>(circle, ECS::Render::Make(CIRCLE, sf::Color(100, 250, 50)));
  
  ECS::EntityID tri = registry.CreateEntity();
  registry.AddComponent<ECS::Transform>(tri, sf::Vector2f{ 200.f, 200.f }, 45.f);
  registry.AddComponent<ECS::Render>(tri, ECS::Render::Make(TRIANGLE, sf::Color::Cyan));
  
  // create the window
  
  //sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "Nitro 2D Engine");

	// set the shape's color to green
	Circle.getShape()->setFillColor(sf::Color(100, 250, 50));

  // run the program as long as the window is open
  while (g_window.isOpen())
  {
    // check all the window's events that were triggered since the last iteration of the loop
    while (const std::optional event = g_window.m_window->pollEvent())
    {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>()) 
      {
        g_window.close();
      }
    }

    float dt = 1.f / 60.f;

    // clear the window with a gray color
    g_window.clear(sf::Color(122, 122, 122));

    // draw everything here...
    Circle.draw(g_window);

    // end the current frame
    g_window.display();
  }
}
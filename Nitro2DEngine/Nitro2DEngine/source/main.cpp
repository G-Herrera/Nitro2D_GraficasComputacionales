#include <Prerequisites.h>
#include <Core/Window.h>
#include <Core/CShape.h>


//Window* g_window = nullptr;
CShape Circle(ShapeType::CIRCLE);


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
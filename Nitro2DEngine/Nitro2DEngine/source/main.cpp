#include <Prerequisites.h>
#include <Core/Window.h>
#include <Core/CShape.h>
#include "ECS/Registry.h"
#include "ECS/EntityFactory.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/Camera.h"
#include "ECS/Systems/RenderSystem.h"
#include "ECS/Systems/CameraSystem.h"
#include "ECS/Systems/UISystem.h"
#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"


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

void destroy() {
  ImGui::SFML::Shutdown();
}


int main()
{
  //Registro de sistemas en el ECS
  registry.AddSystem<ECS::SteeringSystem>();
  registry.AddSystem<ECS::CameraSystem>(g_window);
  registry.AddSystem<ECS::UISystem>();
  registry.AddSystem<ECS::RenderSystem>(g_window);
  

  //m_window es un puntero a sf::RenderWindow.
  if (!ImGui::SFML::Init(*g_window.m_window)) {
    return -1;
  }


  //Habilitar docking
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  sf::Clock deltaClock;
  bool showDemoWindow = true;

  ECS::EntityID circle = ECS::CreateEntity(registry, "Player", { 400.f, 300.f });
  registry.AddComponent<ECS::Render>(circle, ECS::Render::Make(CIRCLE, sf::Color(100, 250, 50), "Textures/wallpaper11.jpg"));

  ECS::EntityID tri = ECS::CreateEntity(registry, "Triangle", { 200.f, 200.f });
  registry.GetComponent<ECS::Transform>(tri).rotation = 45.f;
  registry.AddComponent<ECS::Render>(tri, ECS::Render::Make(TRIANGLE, sf::Color::Cyan));

  ECS::EntityID cam = ECS::CreateEntity(registry, "MainCamera");
  auto& camComp = registry.AddComponent<ECS::Camera>(cam);
  camComp.followTarget = circle;
  camComp.followSpeed = 5.f;
  camComp.zoom = 1;

	// add velocity and acceleration components to the triangle entity
  registry.AddComponent<ECS::Velocity>(tri);
  registry.AddComponent<ECS::Acceleration>(tri);
  auto& triSteer = registry.AddComponent<ECS::SteeringComponent>(tri);

  // Testing steering behaviors
  triSteer.seekEnabled = false;
  triSteer.wanderEnabled = true;

  //triSteer.target = circle;
  triSteer.maxSpeed = 100.f;
  triSteer.maxForce = 60.f;

  triSteer.wanderRadius = 35.f;
  triSteer.wanderDistance = 70.f;
  triSteer.wanderJitter = 2.0f;

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
      //ImGui debe recibir todos los eventos de SFML
      ImGui::SFML::ProcessEvent(*g_window.m_window, *event);
       
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>()) 
      {
        g_window.close();
      }
      //Vector 2 de unsigned int
      else if (const auto* resized = event->getIf<sf::Event::Resized>()) 
      {
        g_window.handleResize(resized->size);
      }
    }

    const sf::Time elapsedTime = deltaClock.restart();
    const float dt = elapsedTime.asSeconds();

    //Iniciar el frame de ImGui
    ImGui::SFML::Update(*g_window.m_window, elapsedTime);

    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), dockspaceFlags);

    // clear the window with a gray color
    g_window.clear(sf::Color(46, 51, 51));

    // draw everything here...
    registry.UpdateSystems(dt);

    //Renderizar ImGui después de la escena.
    ImGui::SFML::Render(*g_window.m_window);

    // end the current frame
    g_window.display();
  }
  destroy();
  return 0;
}
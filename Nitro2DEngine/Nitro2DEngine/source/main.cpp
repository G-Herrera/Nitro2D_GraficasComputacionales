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
ECS::EntityID selectedEntity = ECS::NULL_ENTITY;

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
      //ImGui debe recibir todos los eventos de SFML
      ImGui::SFML::ProcessEvent(*g_window.m_window, *event);
       
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>()) 
      {
        g_window.close();
      }
    }

    const sf::Time elapsedTime = deltaClock.restart();
    const float dt = elapsedTime.asSeconds();

    //Iniciar el frame de ImGui
    ImGui::SFML::Update(*g_window.m_window, elapsedTime);

    ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_PassthruCentralNode;

    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), dockspaceFlags);

    ImGui::Begin("Hierarchy");

    for (auto entity : registry.GetAllEntities()) {
      if (!registry.IsAlive(entity)) continue;

      std::string label = "Entity " + std::to_string(entity);

      if (ImGui::Selectable(label.c_str(), selectedEntity == entity)) 
      {
        selectedEntity = entity;
      }
    }

    ImGui::End();

    ImGui::Begin("Inspector");
    if (registry.IsAlive(selectedEntity)) 
    {
      auto& transform = registry.GetComponent<ECS::Transform>(selectedEntity);

      ImGui::Text("Transform");

      ImGui::DragFloat2("Position", (float*)&transform.position, 1.0f);
      ImGui::DragFloat("Rotation", &transform.rotation, 1.0f);
      ImGui::DragFloat2("Scale", (float*)&transform.scale, 0.01f);
    }
    else
    {
      ImGui::Text("No hay entidad seleccionada");
    }

    if (registry.HasComponent<ECS::Render>(selectedEntity))
    {
      auto& render = registry.GetComponent<ECS::Render>(selectedEntity);

      ImGui::Separator();
      ImGui::Text("Render");

      float color[4] = {
          render.fillColor.r / 255.f,
          render.fillColor.g / 255.f,
          render.fillColor.b / 255.f,
          render.fillColor.a / 255.f
      };

      if (ImGui::ColorEdit4("Color", color))
      {
        render.fillColor = sf::Color(
          static_cast<uint8_t>(color[0] * 255),
          static_cast<uint8_t>(color[1] * 255),
          static_cast<uint8_t>(color[2] * 255),
          static_cast<uint8_t>(color[3] * 255)
        );

        if (render.shape)
          render.shape->setFillColor(render.fillColor);
      }
    }
    ImGui::End();


    ImGui::ShowDemoWindow(&showDemoWindow);

    // clear the window with a gray color
    g_window.clear(sf::Color(122, 122, 122));

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
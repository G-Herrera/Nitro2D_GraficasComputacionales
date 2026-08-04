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
#include "ECS/Components/PathComponent.h"
#include "ECS/Components/DebugPathComponent.h"
#include "ECS/Systems/DebugRenderSystem.h"
#include "ECS/Components/SteeringDebugComponent.h"
#include "Modules/Math2D.h"


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
  registry.AddSystem<ECS::RenderSystem>(g_window);
  registry.AddSystem<ECS::DebugRenderSystem>(g_window);
  registry.AddSystem<ECS::UISystem>();
  

  //m_window es un puntero a sf::RenderWindow.
  if (!ImGui::SFML::Init(*g_window.m_window)) {
    return -1;
  }


  //Habilitar docking
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

  sf::Clock deltaClock;
  bool showDemoWindow = true;

	//Creación del track de fondo y el path de carrera
  ECS::EntityID track = ECS::CreateTrackBackground(registry, "Textures/Track.png");

  const std::vector<sf::Vector2f> raceLineControlPoints = {
    { -210.0f, -240.0f },
    { 210.0f, -240.0f },
    { 379.7f, -169.7f },
    { 450.0f, 0.0f },
    { 379.7f, 169.7f },
    { 210.0f, 240.0f },
    { -210.0f, 240.0f },
    { -379.7f, 169.7f },
    { -450.0f, 0.0f },
    { -379.7f, -169.7f },
  };
  ECS::EntityID racingPath = ECS::CreateRacingPath(registry, raceLineControlPoints, 90.f);

  auto& pathDebug = registry.AddComponent<ECS::DebugPathComponent>(racingPath);
  pathDebug.enabled = true;
  pathDebug.drawCenterLine = true;
  pathDebug.drawPathRadius = true;
  pathDebug.drawSamplePoints = false;

  std::cout << "Path generado con " << registry.GetComponent<ECS::PathComponent>(racingPath).points.size() << " puntos\n";

  auto& pathComponent = registry.GetComponent<ECS::PathComponent>(racingPath);

  if (pathComponent.points.size() < 2)
  {
    std::cerr << "ERROR: El RacingPath necesita al menos 2 puntos.\n";
    return -1;
  }

  // ======================================================
  // Primer kart autonomo (player)
  // ======================================================

	//Creación del jugador en la posición inicial del path
  const sf::Vector2f playerStartPosition = pathComponent.points.front();
  const sf::Vector2f playerInitialDirection = Math::Normalize(pathComponent.points[1] -
                                                              pathComponent.points[0]);

  ECS::EntityID circle =ECS::CreateEntity(registry, "Player", playerStartPosition);
  registry.AddComponent<ECS::Render>(circle, ECS::Render::Make(CIRCLE, sf::Color(100, 250, 50), "Textures/wallpaper11.jpg"));

	//Asignación de componentes de movimiento y steering al jugador
  auto& playerVelocity = registry.AddComponent<ECS::Velocity>(circle);
  registry.AddComponent<ECS::Acceleration>(circle);
  auto& playerSteering = registry.AddComponent<ECS::SteeringComponent>(circle);

	//Habilitar el debug de steering para el jugador
  auto& playerDebug = registry.AddComponent<ECS::SteeringDebugComponent>(circle);
  playerDebug.enabled = true;

	//Configuración inicial de la velocidad del jugador para que se mueva a lo largo del path
  playerVelocity.velocity =playerInitialDirection * 80.f;

  playerSteering.pathFollowingEnabled = true;
  playerSteering.pathEntity = racingPath;

	// Configuración de separation para el jugador, para evitar colisiones con otros karts
  playerSteering.separationEnabled = true;
  playerSteering.separationRadius = 70.f;
  playerSteering.separationStrength = 1.f;
  
  playerSteering.maxSpeed = 110.f;
  playerSteering.maxForce = 80.f;

  playerSteering.pathAheadDistance = 80.f;

  // ======================================================
  // Segundo kart autonomo
  // ======================================================

	//Creación de un segundo kart en la mitad del path
  const std::size_t kart2StartIndex = pathComponent.points.size() / 2;
  const std::size_t kart2NextIndex =(kart2StartIndex + 1) % pathComponent.points.size();

	//Calcular la posición inicial y la dirección del segundo kart
  const sf::Vector2f kart2StartPosition = pathComponent.points[kart2StartIndex];
  const sf::Vector2f kart2InitialDirection = Math::Normalize(pathComponent.points[kart2NextIndex] -
                                                             pathComponent.points[kart2StartIndex]);

	//Creación del segundo kart y asignación de componente de render
  ECS::EntityID kart2 = ECS::CreateEntity(registry, "Kart 2",kart2StartPosition);
	registry.AddComponent<ECS::Render>(kart2, ECS::Render::Make(CIRCLE, sf::Color(250, 100, 50), ""));
	
  //Asignación de componentes de movimiento y steering al segundo kart
  auto& kart2Velocity = registry.AddComponent<ECS::Velocity>(kart2);
  registry.AddComponent<ECS::Acceleration>(kart2);
  auto& kart2Steering = registry.AddComponent<ECS::SteeringComponent>(kart2);
  auto& kart2Debug = registry.AddComponent<ECS::SteeringDebugComponent>(kart2);
  kart2Velocity.velocity = kart2InitialDirection * 75.f;

	//Habilitar el debug de steering para el segundo kart
  kart2Steering.pathFollowingEnabled = true;
  kart2Steering.pathEntity = racingPath;

  // Configuración de separation para el segundo kart, para evitar colisiones con otros karts
  kart2Steering.separationEnabled = true;
  kart2Steering.separationRadius = 70.f;
  kart2Steering.separationStrength = 1.f;

  kart2Steering.maxSpeed = 105.f;
  kart2Steering.maxForce = 75.f;
  kart2Steering.pathAheadDistance = 75.f;

	// Configuración del debug de steering para el segundo kart
  kart2Debug.enabled = true;
  kart2Debug.velocityScale = 0.5f;
  kart2Debug.forceScale = 1.f;

  // Colores
  kart2Debug.velocityColor = sf::Color(0, 180, 255, 230);
  kart2Debug.predictedPositionColor = sf::Color(200, 255, 255, 230);
  kart2Debug.nearestPathPointColor = sf::Color(100, 180, 255, 230);
  kart2Debug.pathTargetPointColor = sf::Color(255, 220, 0, 230);
  kart2Debug.pathFollowingForceColor = sf::Color(0, 100, 255, 230);
  kart2Debug.separationForceColor = sf::Color(220, 80, 255, 230);
  kart2Debug.finalSteeringForceColor = sf::Color(255, 220, 80, 230);

  ECS::EntityID tri = ECS::CreateEntity(registry, "Triangle", { 200.f, 200.f });
  registry.GetComponent<ECS::Transform>(tri).rotation = 45.f;
  registry.AddComponent<ECS::Render>(tri, ECS::Render::Make(TRIANGLE, sf::Color::Cyan));

  ECS::EntityID rec = ECS::CreateEntity(registry, "Rectangle", {350.f, 200.f });
	registry.AddComponent<ECS::Render>(rec, ECS::Render::Make(RECTANGLE, sf::Color::Red));

  

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
  triSteer.wanderEnabled = false;

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
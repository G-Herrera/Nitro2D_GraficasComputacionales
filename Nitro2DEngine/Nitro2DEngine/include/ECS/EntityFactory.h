#pragma once
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Name.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/PathComponent.h"
#include "Modules/Math2D.h"
// ======================================================
// ECS :: EntityFactory.h
//
// Helper de conveniencia (NO forma parte del ECS core).
// Crea una entidad con Transform + Name por defecto, que
// es el mínimo que el Inspector ya asume que toda entidad
// tiene (ver UISystem::Inspector, que accede a Transform
// sin comprobar HasComponent).
//
// Registry::CreateEntity() sigue existiendo intacto y
// puede seguir usándose directamente si en algún caso se
// necesita una entidad sin estos componentes.
// ======================================================
namespace ECS {

	/**
		* @brief Crea una nueva entidad en el registro con componentes Transform y Name por defecto.
		* 
		* @param registry Referencia al Registry donde se creará la entidad.
		* @param name Nombre opcional para la entidad (por defecto "Entity").
		* @param position Posición opcional para el componente Transform de la entidad (por defecto {0.f, 0.f}).
		* 
		* @return EntityID La ID de la nueva entidad creada.
		* 
		* @note Este método es un helper de conveniencia que asegura que toda entidad creada tenga al menos los componentes Transform y Name,
		*/
	inline EntityID
		CreateEntity(Registry& registry, const std::string& name = "Entity",
			sf::Vector2f position = { 0.f, 0.f }) {
		EntityID entity = registry.CreateEntity();
		registry.AddComponent<Transform>(entity, position);
		registry.AddComponent<Name>(entity, name);
		return entity;
	}

	// Crea la entidad de fondo del circuito: una entidad ESTÁTICA
	// (sin Velocity/Acceleration/SteeringComponent) con un Render
	// rectangular del tamaño EXACTO de la textura cargada, centrado
	// en el origen del mundo (0,0).
	//
	// IMPORTANTE: llamar a esta función ANTES de crear cualquier otra
	// entidad con Render (karts, etc). RenderSystem dibuja en el orden
	// de inserción del ComponentPool<Render>; el circuito debe insertarse
	// primero para quedar detrás de todo lo demás.
	inline EntityID
		CreateTrackBackground(Registry& registry, const std::string& texturePath,
			const std::string& name = "Track") {
		EntityID entity = CreateEntity(registry, name, { 0.f, 0.f });

		Render render = Render::Make(RECTANGLE, sf::Color::White);

		auto rect = std::dynamic_pointer_cast<sf::RectangleShape>(render.shape);
		assert(rect && "CreateTrackBackground: se esperaba un sf::RectangleShape");

		auto texture = std::make_shared<sf::Texture>();
		if (texture->loadFromFile(texturePath)) {
			const sf::Vector2f texSize(static_cast<sf::Vector2f>(texture->getSize()));

			// Redimensiona el rectangulo al tamaño real de la textura
			// ANTES de asignarla: si se asigna primero, SetTexture()
			// calcularía el rect de UV usando el tamaño por defecto de
			// Render::Make (100x50), y la textura quedaria mal mapeada.
			rect->setSize(texSize);
			rect->setOrigin(texSize / 2.f);

			render.SetTexture(texture, true);
		}
		else {
			ERROR("EntityFactory", "CreateTrackBackground",
				("No se pudo cargar la textura: " + texturePath).c_str());
		}

		render.zOrder = -1000; // Fondo: siempre detras de todo, sin importar el orden de creacion

		registry.AddComponent<Render>(entity, std::move(render));
		return entity;
	}

	// Crea la entidad "circuito" (path): a partir de pocos puntos de
	// control, genera una polilinea cerrada y densa (Catmull-Rom) y la
	// guarda en un PathComponent. No lleva Render por defecto: el path
	// en si no se dibuja con el pipeline normal, se visualizara con el
	// sistema de debug draw (Etapa 6).
	inline EntityID
		CreateRacingPath(Registry& registry,
			const std::vector<sf::Vector2f>& controlPoints,
			float pathRadius = 40.f,
			int samplesPerSegment = 20,
			const std::string& name = "RacingPath") {
		EntityID entity = CreateEntity(registry, name, { 0.f, 0.f });

		PathComponent path;
		path.points = Math::BuildClosedCatmullRom(controlPoints, samplesPerSegment);
		path.closed = true;
		path.radius = pathRadius;

		registry.AddComponent<PathComponent>(entity, std::move(path));
		return entity;
	}
}
#pragma once
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Name.h"
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
	inline EntityID
		CreateEntity(Registry& registry, const std::string& name = "Entity",
			sf::Vector2f position = { 0.f, 0.f }) {
		EntityID entity = registry.CreateEntity();
		registry.AddComponent<Transform>(entity, position);
		registry.AddComponent<Name>(entity, name);
		return entity;
	}
}
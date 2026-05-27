#pragma once
#include "Prerequisites.h"

// =================================
// ECS:: Types.h
// Tipos fundamentales para el ECS, como ComponentID, EntityID, SystemID, etc.
// Garantiza la unicidad de los IDs y proporciona typedefs para mejorar la legibilidad del código.
//
// EntityID = uint64_t que empaqueta:
//						bits [0..31] -> EntityIndex (posición en el array)
//						bits [32..63] -> EntityVersion (para evitar colisiones de IDs reciclados)
//
// Al destruir una entidad, se incrementa su EntityVersion, asegurando que cualquier ID antiguo 
// (con la versión anterior) sea considerado inválido.

namespace ECS {
	//Tipos primitivos

	using EntityIndex = uint32_t; // Índice de la entidad en el array
	using EntityVersion = uint32_t; // Versión de la entidad para evitar colisiones de IDs reciclados
	using EntityID = uint64_t; // ID completo de la entidad (index + version)
	using ComponentTypeID = uint32_t; // ID de componente único
	
	//Valor centinela para "ninguna entidad"
	inline constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();

	//Empaquetado y desempaquetado de EntityID
	[[nodiscard]] inline EntityIndex GetEntityIndex(EntityID id) noexcept {
		return static_cast<EntityIndex>(id & 0xFFFFFFFF);
	}

	[[nodiscard]] inline EntityVersion GetEntityVersion(EntityID id) noexcept {
		return static_cast<EntityVersion>(id >> 32) & 0xFFFF'FFFFull;
	}

	[[nodiscard]] inline EntityID MakeEntityID(EntityIndex index, EntityVersion version) {
		return (static_cast<EntityID>(version) << 32) | static_cast<EntityID>(index);
	}

	// Generador de IDs de tipo componente
	// Cada tipo T obtiene un ID único en tiempo de ejecución
	// la primera vez que se llama a GetComponenetTypeID<T>().

	[[nodiscard]] inline ComponentTypeID NexComponentTypeID() noexcept {
		static ComponentTypeID counter = 0;
		return counter++;
	}

	template<typename T>
	[[nodiscard]] ComponentTypeID GetComponentTypeID() noexcept {
		static const ComponentTypeID id = NexComponentTypeID();
		return id;
	}
}
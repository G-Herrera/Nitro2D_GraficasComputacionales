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

/**
	* @brief Tipos fundamentales para el ECS.
	* Este archivo define los tipos básicos utilizados en el sistema ECS, como EntityID, ComponentTypeID, etc.
	* EntityID es un uint64_t que empaqueta un EntityIndex (bits 0-31) y un EntityVersion (bits 32-63) para garantizar la unicidad de las entidades incluso después de ser recicladas.
	* ComponentTypeID es un uint32_t que se asigna de forma única a cada tipo de componente en tiempo de ejecución.
	* NULL_ENTITY es un valor especial que representa una entidad nula o inválida.
	* El sistema de generación de IDs de componentes asegura que cada tipo de componente tenga un ID único, lo que facilita la gestión y el acceso a los componentes en las entidades.
	* Estos tipos son fundamentales para la implementación del ECS, ya que permiten una gestión eficiente y segura de las entidades y sus componentes.
	* 
	* @file Types.h
	* 
	* 
	*/

namespace ECS {
	//Tipos primitivos
	/**
	 * @brief Tipos primitivos para el ECS.
	 *
	 * Estos tipos son utilizados internamente por el sistema ECS para identificar entidades y componentes.
	 */
	using EntityIndex = uint32_t; // Índice de la entidad en el array
	using EntityVersion = uint32_t; // Versión de la entidad para evitar colisiones de IDs reciclados
	using EntityID = uint64_t; // ID completo de la entidad (index + version)
	using ComponentTypeID = uint32_t; // ID de componente único
	
	//Valor centinela para "ninguna entidad"

	/**
	 * @brief Valor centinela para "ninguna entidad".
	 *
	 * Este valor se utiliza para representar una entidad nula o inválida en el sistema ECS.
	 */
	inline constexpr EntityID NULL_ENTITY = std::numeric_limits<EntityID>::max();

	//Empaquetado y desempaquetado de EntityID
	/**
		*	@brief Funciones de empaquetado y desempaquetado de EntityID.
		* Estas funciones permiten empaquetar y desempaquetar los componentes de un EntityID.
		*
		* @param id El EntityID a desempaquetar.
		* @return El índice de la entidad.
		*/
	[[nodiscard]] inline EntityIndex GetEntityIndex(EntityID id) noexcept {
		return static_cast<EntityIndex>(id & 0xFFFFFFFF);
	}

	/**
		* @brief Función para obtener la versión de una entidad a partir de su EntityID.
		*
		* Esta función extrae la versión de la entidad del EntityID, lo que es útil para evitar colisiones de IDs reciclados.
		*
		* @param id El EntityID del cual se desea obtener la versión.
		* @return La versión de la entidad.
		*/
	[[nodiscard]] inline EntityVersion GetEntityVersion(EntityID id) noexcept {
		return static_cast<EntityVersion>(id >> 32) & 0xFFFF'FFFFull;
	}

	/**
		* @brief Función para crear un EntityID a partir de su índice y versión.
		*
		* Esta función combina el índice y la versión de la entidad en un único EntityID.
		*
		* @param index El índice de la entidad.
		* @param version La versión de la entidad.
		* @return El EntityID resultante.
		*/
	[[nodiscard]] inline EntityID MakeEntityID(EntityIndex index, EntityVersion version) {
		return (static_cast<EntityID>(version) << 32) | static_cast<EntityID>(index);
	}

	// Generador de IDs de tipo componente
	// Cada tipo T obtiene un ID único en tiempo de ejecución
	// la primera vez que se llama a GetComponenetTypeID<T>().

	/*
	 * @brief Generador de IDs de tipo componente.
	 *
	 * Cada tipo T obtiene un ID único en tiempo de ejecución
	 * la primera vez que se llama a GetComponenetTypeID<T>().
	 */
	[[nodiscard]] inline ComponentTypeID NextComponentTypeID() noexcept {
		static ComponentTypeID counter = 0;
		return counter++;
	}


	/*
	 * @brief Función para obtener el ID de tipo de componente de un tipo dado.
	 *
	 * Esta función utiliza un mecanismo de generación de IDs para asignar un ID único
	 * a cada tipo de componente en tiempo de ejecución.
	 *
	 * @tparam T El tipo de componente para el cual se desea obtener el ID.
	 * @return El ID de tipo de componente único.
	 */
	template<typename T>
	[[nodiscard]] ComponentTypeID GetComponentTypeID() noexcept {
		static const ComponentTypeID id = NextComponentTypeID();
		return id;
	}
}
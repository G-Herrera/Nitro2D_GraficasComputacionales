#pragma once
#include "Prerequisites.h"
#include "ECS/SparseSet.h"

//===============================================================
// ECS :: ComponentPool
//
// IComponentPool - interfaz polimórfica sin tipo para que  Registry
// pueda gestionar pools heterogénenos.
// 
// ComponentPool<T> - almacena compoonenetes de tipo T en una dense array paralelo al de
// SpartSet.
// El Remove usa swap-with-last igual que el SparseSet para mantener los dos arrays sincronizados.
//===============================================================

/**
	* ComponentPool es una clase que se encarga de almacenar los componentes de un tipo específico en el ECS.
	* Cada tipo de componente tiene su propio ComponentPool, que se encarga de gestionar la memoria y el acceso a los componentes de ese tipo.
	* ComponentPool utiliza un vector para almacenar los componentes en un orden paralelo al de las entidades en el SparseSet, 
	* lo que permite un acceso eficiente a los componentes asociados a cada entidad.
	* La eliminación de componentes se realiza mediante la técnica de swap-with-last, que mantiene los arrays sincronizados y evita fragmentación de memoria.
	* ComponentPool también proporciona métodos para agregar, obtener y eliminar componentes, 
	* así como para acceder a todos los componentes de ese tipo, lo que es útil para sistemas que necesitan procesar todos los componentes de un tipo específico.
	*/

namespace ECS{
	//Interfaz polimórfica sin tipo
	class 
	IComponentPool : public SparseSet {
	public:
		~IComponentPool() = default;

		//Elimina el componente de la entidad si existe
		virtual void 
		RemoveEntity(EntityID entity) = 0;

		//PUntireo sin tipo al componenete (para el Serializer)
		virtual void* 
		GetRaw(EntityID entity) noexcept = 0;

	private:

	};

	template<typename T>
	class
		ComponentPool final : public IComponentPool {
	public:
		/**
			* Brief: Agrega un componente de tipo T a la entidad especificada.
			*
			* <Args> args: Argumentos para construir el componente.
			* <Returns> T&: Referencia al componente recién añadido.
			*
			* <Notes> Este método asume que la entidad no tiene ya un componente de este tipo.
			*/
		template<typename... Args> T&
			Add(EntityID entity, Args&&... args) {
			assert(!Contains(entity) && "Entity ya tiene un component");
			InsertEntity(entity); //Registra en sparse/dense y devuelve el denseIndex
			m_components.emplace_back(std::forward<Args>(args)...); //Añade el componente al final del array
			return m_components.back(); //Devuelve referencia al componente recién añadido
		}

		/**
			* Brief: Obtiene una referencia constante al componente de tipo T asociado a la entidad especificada.
			* <Args> entity: ID de la entidad de la que se desea obtener el componente.
			* <Returns> const T&: Referencia constante al componente asociado a la entidad.
			*
			* <Notes> Este método asume que la entidad tiene un componente de este tipo.
			*/
		[[nodiscard]] const T&
			Get(EntityID entity) noexcept {
			assert(Contains(entity) && "Entity no tiene este componente");
			return m_components[m_sparse[GetEntityIndex(entity)]];
		}

		/**
			* Brief: Obtiene una referencia constante al componente de tipo T asociado a la entidad especificada.
			* <Args> entity: ID de la entidad de la que se desea obtener el componente.
			* <Returns> const T&: Referencia constante al componente asociado a la entidad.
			*
			* <Notes> Este método asume que la entidad tiene un componente de este tipo.
			*/
		[[nodiscard]] const T&
			Get(EntityID entity) const noexcept {
			assert(Contains(entity) && "Entity no tiene este componente");
			return m_components[m_sparse[GetEntityIndex(entity)]];
		}

		/**
			* Brief: Intenta obtener un puntero al componente de tipo T asociado a la entidad especificada.
			* <Args> entity: ID de la entidad de la que se desea obtener el componente.
			* <Returns> T*: Puntero al componente asociado a la entidad, o nullptr si no existe.
			*
			* <Notes> Este método no asume que la entidad tenga un componente de este tipo.
			*/
		[[nodiscard]] T*
			TryGet(EntityID entity) noexcept {
			if (!Contains(entity)) return nullptr;
			return &m_components[m_sparse[GetEntityIndex(entity)]];
		}

		// --- Eliminación (swap-with-last)--------------
		// IMPORTANTE: Primero sincronizamos m_componenet y luego llamamos a 
		// SparseSet::Remove para mantener los arrays sincronizados con m_dense
		// Ambos swap usan el mismo denseIndex, así quedan alineados.

		/**
			* Brief: Elimina el componente de tipo T asociado a la entidad especificada.
			* <Args> entity: ID de la entidad de la que se desea eliminar el componente.
			* <Returns> void
			*
			* <Notes> Este método no asume que la entidad tenga un componente de este tipo.
			*/
		void
			Remove(EntityID entity) override {
			if (!Contains(entity)) return;

			const EntityIndex denseIdx = m_sparse[GetEntityIndex(entity)];

			//Mueve el último componente al hueco
			m_components[denseIdx] = std::move(m_components.back());
			m_components.pop_back();

			//Sincroniza el SparseSet
			SparseSet::Remove(entity);
		}

		/**
			* Brief: Elimina el componente de tipo T asociado a la entidad especificada.
			* <Args> entity: ID de la entidad de la que se desea eliminar el componente.
			* <Returns> void
			*
			* <Notes> Este método no asume que la entidad tenga un componente de este tipo.
			*/
		void
		RemoveEntity(EntityID entity) override { Remove(entity); }

		/**
			* Brief: Obtiene un puntero al componente de tipo T asociado a la entidad especificada.
			* <Args> entity: ID de la entidad de la que se desea obtener el componente.
			* <Returns> T*: Puntero al componente asociado a la entidad, o nullptr si no existe.
			*
			* <Notes> Este método no asume que la entidad tenga un componente de este tipo.
			*/
		void* 
		GetRaw(EntityID entity) noexcept override { return TryGet(entity); }

		// Acceso masico (útil para el Serializer / sitemas)
		/**
			* Brief: Obtiene todos los componentes de tipo T.
			* <Returns> std::vector<T>&: Referencia a un vector que contiene todos los componentes de tipo T.
			*/
		[[nodiscard]] std::vector<T>& 
		GetComponents() noexcept { return m_components; }

		/**
			* Brief: Obtiene todos los componentes de tipo T.
			* <Returns> const std::vector<T>&: Referencia a un vector que contiene todos los componentes de tipo T.
			*/
		[[nodiscard]] const std::vector<T>&
		GetComponents() const noexcept { return m_components; }

		/**
			* Brief: Elimina todos los componentes de tipo T.
			* <Returns> void
			*/
		void 
		Clear() override {
			m_components.clear();
			SparseSet::Clear();
		}


	private:
		std::vector<T> m_components; //Paralelo a m_dense de SparseSet, mantiene los componentes en el mismo orden que las entidades en m_dense
	};

}
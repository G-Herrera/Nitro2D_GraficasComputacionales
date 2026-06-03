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
		ComponentPool final : public IComponenetPool {
	public:
		ComponentPool final : public IComponenetPool();
		~ComponentPool final : public IComponenetPool();

		//Añadir
		template<typename... Args> T&
			Add(EntityID entity, Args&&... args) {
			assert(!Contains(entity) && "Entity ya tiene un component");
			InsertEntity(entity); //Registra en sparse/dense y devuelve el denseIndex
			m_components.emplace_back(std::forward<Args>(args)...); //Añade el componente al final del array
			return m_components.back(); //Devuelve referencia al componente recién añadido
		}

		//Obtener
		[[nodiscard]] const T&
			Get(EntityID entity) noexcept {
			assert(Contains(entity) && "Entity no tiene este componente");
			return m_components[m_parse[GetEntityINdex(entity)]];
		}

		[[nodiscard]] const T&
			Get(EntityID entity) const noexcept {
			assert(Contains(entity) && "Entity no tiene este componente");
			return m_components[m_parse[GetEntityINdex(entity)]];
		}

		//Devuleve nullptr si la entidad no tiene el componente
		[[nodiscard]] T*
			TryGet(EntityID entity) noexcept {
			if (!Contains(entity)) return nullptr;
			return &m_components[m_parse[GetEntityIndex(entity)]];
		}

		// --- Eliminación (swap-with-last)--------------
		// IMPORTANTE: Primero sincronizamos m_componenet y luego llamamos a 
		// SparseSet::Remove para mantener los arrays sincronizados con m_dense
		// Ambos swap usan el mismo denseIndex, así quedan alineados.

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

		void
		RemoveEntity(EntityID entity) override { Remove(entity); }

		void* 
		GetRaw(EntityID entity) noexcept override { return TryGet(entity); }

		// Acceso masico (útil para el Serializer / sitemas)
		[[nodiscard]] std::vector<T>& 
		GetComponents() noexcept { return m_components; }

		[[nodiscard]] const std::vector<T>&
		GetComponents() const noexcept { return m_components; }

		void 
		Clear() override {
			m_components.clear();
			SparseSet::Clear();
		}


	private:
		std::vector<T> m_components; //Paralelo a m_dense de SparseSet, mantiene los componentes en el mismo orden que las entidades en m_dense
	};

}
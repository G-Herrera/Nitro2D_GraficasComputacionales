#pragma once
#include "ECS/ComponentPool.h"

/*
* @brief Clase que representa una vista de componentes en el sistema ECS.
* @details La clase View permite iterar sobre entidades que tienen un conjunto específico de componentes.
* @tparam Components Los tipos de componentes que se incluirán en la vista.
*/

namespace ECS {
	template<typename... Components>
	class View
	{
	public:
		/**
		* @brief Constructor de la clase View.
		* @param pools Las pools de componentes que se utilizarán en la vista.
		*/
		explicit View(ComponentPool<Components>&... pools) noexcept : m_pools(pools ...) {
			FindSmallest();
		}

		//Iteración principal
			//Callback con referencia a cada componente

		/**
		* @brief Itera sobre cada entidad en la vista y aplica una función a sus componentes.
		* @param func La función a aplicar a cada entidad y sus componentes.
		*/
		template<typename Func> 
		void Each(Func&& func) {
			if (!m_smallest) return; //No hay componenetes, no iteramos

			cons auto& entities = m_smallest->GetEntities();
			//Recorrido inverso -> seguro al eliminar durante la iteración
			for(std::size_t i = entities.size(); i > 0; i--) {
				const EntityID entity = entities[i - 1];
				if (AllHave(entity)) {
					std::apply([&](auto*... pools) {
						func(entity, pools->Get(entity)...);
					}, 
					m_pools);
				}
			}
		}

		//Iteración solo de entidades
		 //Callback con referencia a cada componente

		/*
		* @brief Itera sobre cada entidad en la vista y aplica una función a sus componentes.
		* @param func La función a aplicar a cada entidad y sus componentes.
		*/
		template<typename Func>
		void EachEntity(Func&& func) {
			if (!m_smallest) return; //No hay componenetes, no iteramos
			const auto& entities = m_smallest->GetEntities();
			//Recorrido inverso -> seguro al eliminar durante la iteración
			for(std::size_t i = entities.size(); i > 0; i--) {
				const EntityID entity = entities[i - 1];
				if (AllHave(entity)) {
					func(entity);
				}
			}
		}

		/**
		* @brief Verifica si la vista está vacía.
		* @return true si la vista está vacía, false en caso contrario.
		*/
		[[nodiscard]] bool Empty() const noexcept { return !m_smallest || m_smallest->Empty(); }

		/*
		* @brief Devuelve el número de entidades en la vista.
		* @return El número de entidades en la vista.
		*/
		[[nodiscard]] std::size_t Size() const noexcept {return m_smallest ? m_smallest->Size() : 0;}
	private:
		/**
		* @brief Busca la pool de componentes más pequeña.
		*/
		template <std::size_t I = 0>
		void FindSmallest() {
			if constexpr (I < sizeof...(Components)) {
				auto& pool = std::get<I>(m_pools);
				if (pool &&(!m_smallest || pool->Size() < m_smallest->Size())) {
					m_smallest = pool;
					FindSmallest<I + 1>();
				}
				
			}
		}

		/**
		* @brief Verifica si todos los pools de componentes contienen la entidad especificada.
		* @param entity La ID de la entidad a verificar.
		* @return true si todos los pools contienen la entidad, false en caso contrario.
		*/
		[[nodiscard]] bool 
		AllHave(EntityID entity) const noexcept {
			return std::apply
			([entity](auto&... pools) noexcept {
				return (... && (pools && pools->Contains(entity)));
			}, 
			m_pools);


		}

	private:
		std::tuple<ComponentPool<Components>&...> m_pools; ///< Las pools de componentes que se utilizarán en la vista.
		const SparseSet* m_smallest = nullptr; ///< La pool de componentes más pequeña, utilizada para optimizar la iteración.

	};

	
}
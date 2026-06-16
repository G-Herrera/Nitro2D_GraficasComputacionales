#pragma once
#include "Prerequisites.h"
#include "ECS/Types.h"

/**
	* @brief Clase base para conjuntos dispersos (Sparse Sets) en un sistema de entidades y componentes (ECS).
	* @details La clase SparseSet proporciona una implementación eficiente para almacenar y gestionar entidades en un sistema ECS. 
	* Utiliza dos vectores: uno "sparse" que mapea índices de entidades a índices densos, y otro "dense" que almacena las entidades de manera contigua. 
	* Esto permite operaciones rápidas de inserción, eliminación y consulta de entidades.
	* 
	* @tparam T El tipo de entidad que se almacenará en el conjunto disperso.
	*
	* @note Esta clase es genérica y puede ser utilizada con cualquier tipo de entidad que cumpla con los requisitos del sistema ECS.
	*/

namespace ECS {
	class 
	SparseSet {
	public:
		SparseSet() = default;
		virtual ~SparseSet() = default;

		//Consultas

		/*
		* @brief Verifica si una entidad está contenida en el conjunto disperso.
		* @param entity La ID de la entidad a verificar.
		* @return true si la entidad está en el conjunto, false en caso contrario.
		*/
		[[nodiscard]] bool Contains(EntityID entity) const noexcept 
		{
			const EntityIndex idx = GetEntityIndex(entity);
			if (idx >= m_sparse.size()) return false;
			const EntityIndex denseIdx = m_sparse[idx];
			return denseIdx < m_dense.size() && m_dense[denseIdx] == entity;
		}

		/*
		* @brief Devuelve el número de entidades en el conjunto disperso.
		* @return El número de entidades en el conjunto.
		*/
		[[nodiscard]] size_t Size()  const noexcept { return m_dense.size(); }

		/*
		* @brief Verifica si el conjunto disperso está vacío.
		* @return true si el conjunto está vacío, false en caso contrario.
		*/
		[[nodiscard]] bool	 Empty() const noexcept { return m_dense.empty(); }

		/*
		* @brief Devuelve una referencia constante al vector de entidades en el conjunto disperso.
		* @return Una referencia constante al vector de entidades.
		*/
		[[nodiscard]] const std::vector<EntityID>& GetEntities() const noexcept 
		{
			return m_dense;
		}

		//Eliminación (swap-with-last)
		//Las subclases DEBEN llamar a esta base DESPUÉS de
		//sincronizar sus propios arrays (ver ComponentPool::Remove)

		/**
		* @brief Elimina una entidad del conjunto disperso.
		* @param entity La ID de la entidad a eliminar.
		*/
		virtual void Remove(EntityID entity) 
		{
			if (!Contains(entity)) return;

			const EntityIndex sparseIdx = GetEntityIndex(entity);
			const EntityIndex denseIdx = m_sparse[sparseIdx];
			const EntityID		last = m_dense.back();

			//Mueve el último elemento al hueco
			m_dense[denseIdx] = last;
			m_sparse[GetEntityIndex(last)] = denseIdx;
			m_dense.pop_back();

			//Invalida la entrada eliminada
			m_sparse[sparseIdx] = INVALID;
		}

		/**
		* @brief Elimina todas las entidades del conjunto disperso.
		*/
		virtual void Clear() 
		{
			m_sparse.clear();
			m_dense.clear();
		}

	protected:
		//Reserva espacio m_sparse y registra la entidad en m_dense.
		//Devuelve el denseIndex asignado.

		/*
		* @brief Inserta una entidad en el conjunto disperso.
		* @param entity La ID de la entidad a insertar.
		* @return El índice denso asignado a la entidad.
		*/
		EntityIndex InsertEntity(EntityID entity) 
		{
			const EntityIndex sparseIdx = GetEntityIndex(entity);
			const EntityIndex denseIdx = static_cast<EntityIndex>(m_dense.size());

			if (sparseIdx >= m_sparse.size()) m_sparse.resize(sparseIdx + 1, INVALID);

			assert(m_sparse[sparseIdx] == INVALID && "La entidad ya está en el set");

			m_sparse[sparseIdx] = denseIdx;
			m_dense.push_back(entity);

			return denseIdx;
		}

	protected:
		static constexpr EntityIndex INVALID = std::numeric_limits<EntityIndex>::max(); /// Valor centinela para entradas no válidas en el vector sparse.

		std::vector<EntityIndex> m_sparse; //sparse[entityIndex] -> dense index 
		std::vector<EntityID> m_dense; //dense[i] -> EntityID
	};

}
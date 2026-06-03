#pragma once
#include "ECS/ComponentPool.h"
#include "ECS/View.h"
#include "ECS/Types.h"
#include "ECS/System.h"

namespace ECS {
	class 
	Registry {
	public:
		Registry();
		~Registry();

	private:
		//Entidades
		std::vector<EntityID> m_entities; //Almacena todas las entidades activas
		std::vector<EntityID> m_versions; //Almacena la version de cada entidad para evitar colisiones de IDs reciclados
		std::queue<EntityID> m_freeList; //Almacena IDs de entidades eliminadas para reciclar

		//Componentes
		std::unordered_map<ComponentTypeID, std::unique_ptr<IComponentPool>> m_componentPools; //Mapea el tipo de componente a su pool correspondiente

		//Sistemas
		std::vector<std::unique_ptr<System>> m_systems; //Almacena los sistemas registrados en el orden de actualización
	};


}
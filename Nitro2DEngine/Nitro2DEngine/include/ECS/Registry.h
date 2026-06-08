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

		EntityID
			CreateEntity() {
			EntityID idx;
			if (!m_freeList.empty()) {
				idx = m_freeList.front();
				m_freeList.pop();
			}
			else {
				idx = static_cast<EntityID>(m_versions.size());
				m_versions.push_back(0); //Nueva entidad, version 0
				m_entities.push_back(idx); //Agrega a entidades activas
			}

			EntityID id = MakeEntityID(idx, m_versions[idx]);
			m_entities[idx] = id; //Actualiza con el ID completo
			return id;
		};

		void
			DestroyEntity(EntityID entity) {
			assert(IsAlive(entity) && "DestroyEntity: Entity no existe o ya fue destruida");

			//Elimina todos los componentes de esta entidad
			for (auto& [typeID, pool] : m_componentPools) {
				pool->RemoveEntity(entity);
			}

			//Incrementa versión -> los IDs viejos quedan inválidos
			const EntityID idx = GetEntityIndex(entity);
			++m_versions[idx];
			m_entities[idx] = NULL_ENTITY; //Marca como inactiva
			m_freeList.push(idx); //Agrega a la lista de reciclaje
		}

		//Componentes
		[[nodiscard]] bool
			IsAlive(EntityID entity) const noexcept {
			const EntityID idx = GetEntityIndex(entity);
			return idx < m_entities.size() && m_entities[idx] == entity;
		}

		[[nodiscard]] std::size_t
			EntityCount() const noexcept { return m_entities.size() - m_freeList.size(); }

		//Todas las ranuras (incluye NULL_ENTITY para los huecos libres).
		//Útil pra el Serializer; filtra con IsAlive() para iterar solo las entidades activas.
		[[nodiscard]] const std::vector<EntityID>&
			GetAllEntities() const noexcept { return m_entities; }

		//Añade un componente a una entidad, creando el pool si no existe
		//Acepta argumentos de construcción directos (perfect - forward)
		template<typename T, typename... Args> T&
			AddComponent(EntityID entity, Args&&... args) {
			assert(IsAlive(entity) && "AddComponent: Entity no existe o ya fue destruida");
			return GetOrCreatePool<T>()->Add(entity, std::forward<Args>(args)...);
		}

		//Elimina el componenete T de la entidad (no-op si no existe)
		template<typename T> void
			RemoveComponent(EntityID entity) {

			if (auto* pool = GetPool<T>()) {
				pool->Remove(entity);
			}

		}

		//Remplaza el componente T de la entidad, o lo añade si no existe
		template<typename T, typename... Args> T&
			SetComponent(EntityID entity, T value) {
			assert(IsAlive(entity) && "SetComponent: Entity no existe o ya fue destruida");

			auto* pool = GetOrCreatePool<T>();

			if (pool->Contains(entity)) {
				pool->Get(entity) = std::move(value);
				return pool->Get(entity);
			}

			return pool->Add(entity, std::move(value));
		}

		template<typename T>
		[[nodiscard]] bool HasComponent(EntityID entity) const noexcept {

			const auto* pool = GetPoolConst<T>();
			return pool && pool->Contains(entity);

		}

		//Obtiene una referencia al componente T de la entidad. Lanza aserción si no existe.
		//Acceso garantizado (assert si no existe)
		template<typename T>
		[[nodiscard]] T& GetComponent(EntityID entity) {
			assert(IsAlive(entity));
			auto* pool = GetPool<T>();
			assert(pool && "GetComponent: Entity no tiene este componente");
			return pool->Get(entity);
		}
		//Versión const de GetComponent
		template<typename T>
		[[nodiscard]] const T& GetComponent(EntityID entity) const {
			assert(IsAlive(entity));
			const auto* pool = GetPoolConst<T>();
			assert(pool && "GetComponent: Entity no tiene este componente");
			return pool->Get(entity);
		}

		//Acceso seguro: devuelve puntero o nullptr si no existe
		template<typename T>
		[[nodiscard]] T* TryGetComponent(EntityID entity) noexcept {
			if (!IsAlive(entity)) return nullptr;
			auto* pool = GetPool<T>();
			return pool ? pool->TryGet(entity) : nullptr;
		}

		//Views (queries mult-componente)
		//Ejemplo: registry.GetView<Transform, Velocity>()
		template<typename... Components>
		[[nodiscard]] View<Components...> GetView() noexcept {
			return View<Components...>(GetOrCreatePool<Components>()...);
		}

		//Sistemas
		template<typename T, typename... Args> T&
			AddSystem(Args&&... args) {
			static_assert(std::is_base_of_v<System, T>, "T debe ser un System");
			auto system = std::make_unique<T>(std::forward<Args>(args)...);
			T& ref = *system;
			system->OnStart(*this); //Llama a OnStart al registrarse
			m_systems.push_back(std::move(system)); //Agrega el sistema al vector
			return ref;
		}

		void
			UpdateSystems(float deltaTime) {
			for (auto& system : m_systems) {
				if (system->IsEnabled()) {
					system->OnUpdate(*this, deltaTime);
				}
			}
		}

		void RemoveAllSystems() {
			for (auto& system : m_systems) {
				system->OnStop(*this);
			}
			m_systems.clear();
		}

		//Utilidades
		//Destruye todo: entidades, componenetes y sistemas
		void Clear() {
			RemoveAllSystems();
			for (auto& [typeID, pool] : m_componentPools) {
				pool->Clear();
			}
			m_entities.clear();
			m_versions.clear();
			while (!m_freeList.empty()) m_freeList.pop();
		}

		//Acceso a pools sin tipo (para el Serializer)
		[[nodiscard]] const std::unordered_map<ComponentTypeID, std::unique_ptr<IComponentPool>>&
			GetPools() const noexcept { return m_componentPools; }

	private:
		//Helpers privados
		template<typename T>
		ComponentPool<T>* GetOrCreatePool() {

			const ComponentTypeID typeID = GetComponentTypeID<T>();
			auto it = m_componentPools.find(typeID);
			
			if (it == m_componentPools.end()) {
				auto [newIt, ok] = m_componentPools.emplace(typeID, std::make_unique<ComponentPool<T>>());
				return static_cast<ComponentPool<T>*>(newIt->second.get());
			}

			return static_cast<ComponentPool<T>*>(it->second.get());
		}

		template<typename T>
		ComponentPool<T>* GetPool() noexcept{
			const ComponentTypeID typeID = GetComponentTypeID<T>();
			auto it = m_componentPools.find(typeID);
			return it != m_componentPools.end() ? static_cast<ComponentPool<T>*>(it->second.get()) : nullptr;
		}

		template<typename T>
		const ComponentPool<T>* GetPoolConst() const noexcept {
			const ComponentTypeID typeID = GetComponentTypeID<T>();
			auto it = m_componentPools.find(typeID);
			return it != m_componentPools.end() ? static_cast<const ComponentPool<T>*>(it->second.get()) : nullptr;
		}

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
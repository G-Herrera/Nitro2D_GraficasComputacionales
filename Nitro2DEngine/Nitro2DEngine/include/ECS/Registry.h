#pragma once
#include "ECS/ComponentPool.h"
#include "ECS/View.h"
#include "ECS/Types.h"
#include "ECS/System.h"

/**
	* @brief Clase que representa el sistema de entidades y componentes.
	*
	* Esta clase es responsable de gestionar la creación, destrucción y acceso a entidades y sus componentes.
	* @note Esta clase no es thread-safe.
	*
	* Se recomienda utilizar esta clase en un contexto de un solo hilo o implementar mecanismos de bloqueo adecuados.
	*/

namespace ECS {
	class
		Registry {
	public:

		/**
		 * @brief Crea una nueva entidad.
		 *
		 * Esta función asigna un nuevo ID de entidad y lo marca como activo.
		 * @return El ID de la nueva entidad.
		 */
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

		/**
		 * @brief Destruye una entidad.
		 *
		 * Esta función marca la entidad como inactiva y elimina todos sus componentes.
		 * @param entity El ID de la entidad a destruir.
		 */
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

		/**
		 * @brief Verifica si una entidad está activa.
		 *
		 * Esta función comprueba si la entidad existe y está activa en el sistema.
		 * @param entity El ID de la entidad a verificar.
		 * @return true si la entidad está activa, false en caso contrario.
		 */
		[[nodiscard]] bool
			IsAlive(EntityID entity) const noexcept {
			const EntityID idx = GetEntityIndex(entity);
			return idx < m_entities.size() && m_entities[idx] == entity;
		}

		/*
		 * @brief Obtiene la cantidad de entidades activas en el sistema.
		 *
		 * Esta función cuenta todas las entidades que están activas y no han sido destruidas.
		 * @return La cantidad de entidades activas.
		 */
		[[nodiscard]] std::size_t
			EntityCount() const noexcept { return m_entities.size() - m_freeList.size(); }

		//Todas las ranuras (incluye NULL_ENTITY para los huecos libres).
		//Útil pra el Serializer; filtra con IsAlive() para iterar solo las entidades activas.

		/*
		 * @brief Obtiene todas las entidades en el sistema.
		 *
		 * Esta función devuelve un vector con todas las entidades, incluyendo
		 * aquellas que han sido destruidas (NULL_ENTITY).
		 * @return Un vector con todos los IDs de entidad.
		 */
		[[nodiscard]] const std::vector<EntityID>&
			GetAllEntities() const noexcept { return m_entities; }

		//Añade un componente a una entidad, creando el pool si no existe
		//Acepta argumentos de construcción directos (perfect - forward)

		/*
		 * @brief Añade un componente a una entidad.
		 *
		 * Esta función crea un nuevo componente del tipo T y lo asocia a la entidad.
		 * @param entity El ID de la entidad a la que se le añadirá el componente.
		 * @param args Los argumentos de construcción para el componente.
		 * @return Una referencia al componente recién creado.
		 */
		template<typename T, typename... Args> T&
			AddComponent(EntityID entity, Args&&... args) {
			assert(IsAlive(entity) && "AddComponent: Entity no existe o ya fue destruida");
			return GetOrCreatePool<T>()->Add(entity, std::forward<Args>(args)...);
		}

		//Elimina el componenete T de la entidad (no-op si no existe)

		/**
		 * @brief Elimina un componente de una entidad.
		 *
		 * Esta función elimina el componente del tipo T asociado a la entidad.
		 * @param entity El ID de la entidad de la que se eliminará el componente.
		 */
		template<typename T> void
			RemoveComponent(EntityID entity) {

			if (auto* pool = GetPool<T>()) {
				pool->Remove(entity);
			}

		}

		//Remplaza el componente T de la entidad, o lo añade si no existe
		/**
		 * @brief Establece un componente de una entidad.
		 *
		 * Esta función reemplaza el componente del tipo T asociado a la entidad,
		 * o lo añade si no existe.
		 * @param entity El ID de la entidad a la que se le establecerá el componente.
		 * @param value El nuevo valor del componente.
		 * @return Una referencia al componente establecido.
		 */
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

		/*
		 * @brief Verifica si una entidad tiene un componente.
		 *
		 * Esta función comprueba si la entidad tiene un componente del tipo T asociado.
		 * @param entity El ID de la entidad a verificar.
		 * @return true si la entidad tiene el componente, false en caso contrario.
		 */
		template<typename T>
		[[nodiscard]] bool HasComponent(EntityID entity) const noexcept {

			const auto* pool = GetPoolConst<T>();
			return pool && pool->Contains(entity);

		}

		//Obtiene una referencia al componente T de la entidad. Lanza aserción si no existe.
		//Acceso garantizado (assert si no existe)

		/**
			* @brief Obtiene un componente de una entidad.
			*
			* Esta función devuelve una referencia al componente del tipo T asociado a la entidad.
			* @param entity El ID de la entidad de la que se obtendrá el componente.
			* @return Una referencia al componente de la entidad.
			*/
		template<typename T>
		[[nodiscard]] T& GetComponent(EntityID entity) {
			assert(IsAlive(entity));
			auto* pool = GetPool<T>();
			assert(pool && "GetComponent: Entity no tiene este componente");
			return pool->Get(entity);
		}

		//Versión const de GetComponent

		/**
			* @brief Obtiene un componente de una entidad (versión constante).
			*
			* Esta función devuelve una referencia constante al componente del tipo T asociado a la entidad.
			* @param entity El ID de la entidad de la que se obtendrá el componente.
			* @return Una referencia constante al componente de la entidad.
			*/ 
		template<typename T>
		[[nodiscard]] const T& GetComponent(EntityID entity) const {
			assert(IsAlive(entity));
			const auto* pool = GetPoolConst<T>();
			assert(pool && "GetComponent: Entity no tiene este componente");
			return pool->Get(entity);
		}

		//Acceso seguro: devuelve puntero o nullptr si no existe

		/**
			* @brief Intenta obtener un componente de una entidad.
			*
			* Esta función devuelve un puntero al componente del tipo T asociado a la entidad,
			* o nullptr si la entidad no tiene ese componente.
			* @param entity El ID de la entidad de la que se intentará obtener el componente.
			* @return Un puntero al componente de la entidad, o nullptr si no existe.
			*/ 
		template<typename T>
		[[nodiscard]] T* TryGetComponent(EntityID entity) noexcept {
			if (!IsAlive(entity)) return nullptr;
			auto* pool = GetPool<T>();
			return pool ? pool->TryGet(entity) : nullptr;
		}

		//Views (queries mult-componente)
		//Ejemplo: registry.GetView<Transform, Velocity>()

		/**
		 * @brief Obtiene una vista de las entidades que tienen los componentes especificados.
		 *
		 * Esta función devuelve una vista que permite iterar sobre todas las entidades que tienen
		 * los componentes del tipo especificado.
		 * @return Una vista de las entidades con los componentes solicitados.
		 */
		template<typename... Components>
		[[nodiscard]] View<Components...> GetView() {
			return View<Components...>(GetOrCreatePool<Components>()...);
		}

		//Sistemas

		/*
		 * @brief Agrega un sistema al registro.
		 *
		 * Esta función crea una instancia del sistema T y lo registra en el
		 * sistema de ECS. Llama a OnStart en el sistema después de registrarlo.
		 * @tparam T El tipo del sistema a agregar.
		 * @tparam Args Los tipos de los argumentos para el constructor del sistema.
		 * @param args Los argumentos para el constructor del sistema.
		 * @return Una referencia al sistema agregado.
		 */
		template<typename T, typename... Args> T&
			AddSystem(Args&&... args) {
			static_assert(std::is_base_of_v<System, T>, "T debe ser un System");
			auto system = std::make_unique<T>(std::forward<Args>(args)...);
			T& ref = *system;
			system->OnStart(*this); //Llama a OnStart al registrarse
			m_systems.push_back(std::move(system)); //Agrega el sistema al vector
			return ref;
		}

		/*
		 * @brief Actualiza todos los sistemas.
		 *
		 * Esta función itera sobre todos los sistemas registrados y llama a su método OnUpdate.
		 * @param deltaTime El tiempo transcurrido desde la última actualización.
		 */
		void
			UpdateSystems(float deltaTime) {
			for (auto& system : m_systems) {
				if (system->IsEnabled()) {
					system->OnUpdate(*this, deltaTime);
				}
			}
		}

		/*
		 * @brief Elimina todos los sistemas registrados.
		 *
		 * Esta función llama a OnStop en cada sistema antes de eliminarlo y limpiar la lista de sistemas.
		 */
		void RemoveAllSystems() {
			for (auto& system : m_systems) {
				system->OnStop(*this);
			}
			m_systems.clear();
		}

		//Utilidades
		//Destruye todo: entidades, componenetes y sistemas

		/*
		 * @brief Limpia completamente el registro.
		 *
		 * Esta función elimina todas las entidades, componentes y sistemas registrados,
		 * dejando el registro en un estado vacío.
		 */
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

		/*
		 * @brief Obtiene todos los pools de componentes.
		 *
		 * Esta función devuelve un mapa que asocia cada tipo de componente con su pool correspondiente.
		 * @return Un mapa de pools de componentes.
		 */
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
#pragma once
#include <vector>
#include <string>
#include <functional>
#include "ECS/Registry.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"
#include "ECS/Components/Obstacle.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Name.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/PathComponent.h"
#include "ECS/Components/PathEditorComponent.h"
#include "ECS/Components/DebugPathComponent.h"
#include "ECS/Components/SteeringDebugComponent.h"
#include "ECS/Components/StartingGridComponent.h"
#include "ECS/Components/RaceParticipantComponent.h"
#include "ECS/Components/MovementControlComponent.h"
#include "ECS/Components/RaceManagerComponent.h"

//=========================================================
// ECS::Editor::ComponentRegistry
//
// Metadatos de EDITOR (no de ECS): tabla estática que
// permite al Inspector ofrecer un botón "+ Add Component"
// genérico, sin que Registry, ComponentPool o View tengan
// que saber nada de tipos concretos.
//
// Cada entrada sabe:
// - Cómo preguntar si una entidad ya tiene el componente
// - Cómo añadirlo (incluyendo dependencias automáticas,
//   ej. SteeringComponent requiere Velocity + Acceleration)
//
// Para registrar un nuevo tipo de componente en el botón
// "Add Component", basta con añadir una línea en el
// constructor de esta clase. No requiere tocar el ECS.
//=========================================================
namespace ECS::Editor {

	struct ComponentTypeInfo {
		std::string name;
		// Dependencias necesarias para que el componente
		// pueda funcionar correctamente.
		std::vector<std::string> requiredComponents;
		std::function<bool(Registry&, EntityID)> hasComponent;
		std::function<void(Registry&, EntityID)> addComponent;
		std::function<void(Registry&, EntityID)> removeComponent;
	};

	class
		ComponentRegistry {
	public:
		/**
			* @brief Devuelve la instancia singleton de ComponentRegistry.
			*
			* @return ComponentRegistry& Referencia a la instancia singleton de ComponentRegistry.
			*/
		static ComponentRegistry&
			Instance() {
			static ComponentRegistry instance;
			return instance;
		}

		/**
			* @brief Devuelve la lista de tipos de componentes registrados.
			*
			* @return const std::vector<ComponentTypeInfo>& Referencia constante a la lista de tipos de componentes registrados.
			*/
		[[nodiscard]] const std::vector<ComponentTypeInfo>&
			GetTypes() const noexcept { return m_types; }

		/**
			* @brief Comprueba si un componente puede eliminarse
			* sin romper dependencias de otros componentes presentes.
			*/
		[[nodiscard]] bool
		CanRemove(Registry& registry,EntityID entity,const ComponentTypeInfo& candidate,
							std::string* blockedBy = nullptr) const
		{
			for (const auto& type : m_types)
			{
				// El componente dependiente ni siquiera existe.
				if (!type.hasComponent(registry, entity)) continue;

				for (const std::string& requirement : type.requiredComponents)
				{
					if (requirement == candidate.name)
					{
						if (blockedBy)
						{
							*blockedBy = type.name;
						}

						return false;
					}
				}
			}

			return true;
		}

	private:
		ComponentRegistry() {
			RegisterCustom(
				"Render",

				// HAS
				[](Registry& registry, EntityID entity)
				{
					return registry.HasComponent<Render>(entity);
				},

				// ADD
				[](Registry& registry, EntityID entity)
				{
					if (registry.HasComponent<Render>(entity)) return;

					registry.AddComponent<Render>(entity, Render::Make(RECTANGLE, sf::Color::White));
				},

				// REMOVE
				[](Registry& registry, EntityID entity)
				{
					if (registry.HasComponent<Render>(entity))
					{
						registry.RemoveComponent<Render>(entity);
					}
				});
			Register<Velocity>("Velocity");
			Register<Acceleration>("Acceleration");
			Register<Obstacle>("Obstacle");
			Register<Camera>("Camera");
			Register<SteeringComponent>("Steering Component", { "Velocity", "Acceleration" });
			Register<SteeringDebugComponent>("Steering Debug", { "Steering Component" });
			Register<Name>("Name");
			Register<PathComponent>("Path");
			Register<DebugPathComponent>("Path Debug");
			Register<PathEditorComponent>("Path Editor",{ "Path", "Path Debug"});
			Register<StartingGridComponent>("Starting Grid");
			Register<RaceParticipantComponent>("Race Participant", {"Movement Control"});
			Register<MovementControlComponent>("Movement Control");
			Register<RaceManagerComponent>("Race Manager");
			
			// Transform no se registra porque toda entidad creada desde
			// EntityFactory ya nace con ese componente y el Inspector
			// actualmente lo considera obligatorio.
		}

		/**
			* @brief Registra un nuevo tipo de componente en el registro.
			* 
			* @tparam T El tipo de componente a registrar.
			* @param name El nombre del componente que se mostrará en el editor.
			* @param requiredNames Una lista de nombres de componentes requeridos que se agregarán automáticamente al agregar este componente.
			* 
			* @note Este método se utiliza internamente en el constructor de ComponentRegistry para registrar los tipos de componentes disponibles en el editor.
			* @note Los componentes requeridos se agregarán automáticamente si no están presentes al agregar este componente.
			*/
		template<typename T>void 
		Register(std::string name, std::vector<std::string> requiredNames = {})
		{
			ComponentTypeInfo info;

			info.name = std::move(name);

			// Conservamos las dependencias como metadatos.
			info.requiredComponents = requiredNames;


			// --------------------------------------------------
			// HAS
			// --------------------------------------------------

			info.hasComponent = [](Registry& registry, EntityID entity)
				{
					return registry.HasComponent<T>(entity);
				};


			// --------------------------------------------------
			// ADD
			// --------------------------------------------------

			info.addComponent = [this, requiredNames](Registry& registry, EntityID entity)
				{
					// Primero agregar las dependencias.
					for (const auto& reqName : requiredNames)
					{
						for (auto& type : m_types)
						{
							if (type.name != reqName)
							{
								continue;
							}

							if (!type.hasComponent(registry, entity))
							{
								type.addComponent(registry, entity);
							}

							break;
						}
					}

					if (!registry.HasComponent<T>(entity))
					{
						registry.AddComponent<T>(entity);
					}
				};


			// --------------------------------------------------
			// REMOVE
			// --------------------------------------------------

			info.removeComponent = [](Registry& registry, EntityID entity)
				{
					if (registry.HasComponent<T>(entity))
					{
						registry.RemoveComponent<T>(entity);
					}
				};


			m_types.push_back(std::move(info));
		}

		/**
			* @brief Registra un componente utilizando funciones personalizadas.
			*
			* Se utiliza para componentes que no pueden añadirse correctamente
			* mediante un constructor por defecto, como Render, que necesita
			* crear una forma visual válida.
			*
			* @param name Nombre mostrado dentro del menú Add Component.
			* @param hasComponent Función que comprueba si la entidad ya posee
			*        el componente.
			* @param addComponent Función que construye y añade el componente.
			*/
		void RegisterCustom(
			std::string name,
			std::function<bool(Registry&, EntityID)> hasComponent,
			std::function<void(Registry&, EntityID)> addComponent,
			std::function<void(Registry&, EntityID)> removeComponent,
			std::vector<std::string> requiredNames = {})
		{
			ComponentTypeInfo info;

			info.name = std::move(name);

			info.requiredComponents = std::move(requiredNames);

			info.hasComponent = std::move(hasComponent);

			info.addComponent = std::move(addComponent);

			info.removeComponent = std::move(removeComponent);

			m_types.push_back(std::move(info));
		}

		std::vector<ComponentTypeInfo> m_types;
	};
}
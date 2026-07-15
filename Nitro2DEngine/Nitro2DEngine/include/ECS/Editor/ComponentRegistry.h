#pragma once
#include "ECS/Registry.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"
#include "ECS/Components/Obstacle.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/Name.h"
#include <vector>
#include <string>
#include <functional>

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
		std::function<bool(Registry&, EntityID)> hasComponent;
		std::function<void(Registry&, EntityID)> addComponent;
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

	private:
		ComponentRegistry() {
			Register<Velocity>("Velocity");
			Register<Acceleration>("Acceleration");
			Register<Obstacle>("Obstacle");
			Register<Camera>("Camera");
			Register<SteeringComponent>("Steering Component", { "Velocity", "Acceleration" });
			Register<Name>("Name");
			// Nota: Transform y Render no se listan aquí porque en este
			// proyecto toda entidad ya nace con Transform, y Render
			// requiere parámetros de construcción (tipo de forma,
			// textura) que no tienen un valor "por defecto" razonable
			// para un botón genérico.
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
		template<typename T> void 
		Register(std::string name, std::vector<std::string> requiredNames = {}) {
			ComponentTypeInfo info;
			info.name = std::move(name);

			info.hasComponent = [](Registry& registry, EntityID entity) {
				return registry.HasComponent<T>(entity);
				};

			info.addComponent = [this, requiredNames](Registry& registry, EntityID entity) {
				// Añade primero las dependencias que falten
				for (const auto& reqName : requiredNames) {
					for (auto& type : m_types) {
						if (type.name == reqName && !type.hasComponent(registry, entity)) {
							type.addComponent(registry, entity);
						}
					}
				}
				if (!registry.HasComponent<T>(entity)) {
					registry.AddComponent<T>(entity);
				}
				};

			m_types.push_back(std::move(info));
		}

		std::vector<ComponentTypeInfo> m_types;
	};
}
#pragma once

//===============================================================
// ECS :: System
//
// Clase base para todos los sistemas del motor. Define la interfaz común para la lógica frame-a-frame y la gestión de recursos.
// 
// Un sistema contiene únicamente LÓGICA, nunca datos.
// Los datos viven en los componentes, y el sistema opera sobre ellos a través de la Registry.
//
// Ciclo de vida de un sistema:
// 1. OnStart: se llama una vez al registrarse el sistema, ideal para inicializar recursos o referencias a componentes.
// 2. OnUpdate: se llama cada frame, aquí va la lógica principal del sistema.
// 3. OnStop: se llama una vez al eliminar el sistema, ideal para limpiar recursos o referencias.
//===============================================================

namespace ECS {

	//Forward declaration para evitar dependencias circulares con Registry
	class Registry;

	class
		System {
	public:
		System();
		virtual ~System() = default;

		//Inicialización del sistema (se llama una vez al registrarse)
		virtual void OnStart(Registry& registry) {}

		//Lógica frame-a-frame (se llama cada frame)
		virtual void OnUpdate(Registry& registry, float deltaTime) = 0;

		//Limpieza al destruir el sistema (se llama una vez al eliminarse)
		virtual void OnStop(Registry& registry) {}

		//Opcional: activa/desactiva el sistema sin eliminarlo (puede ser útil para sistemas de renderizado o lógica que no siempre debe ejecutarse)
		void SetEnabled(bool enabled) noexcept { m_enabled = enabled; }
		[[nodiscard]] bool IsEnabled() const { return m_enabled; }

	private:
		bool m_enabled = true;

	};

}
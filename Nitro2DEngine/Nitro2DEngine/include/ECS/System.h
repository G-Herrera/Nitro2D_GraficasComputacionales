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

/**
	* @file System.h
	* @brief Clase base para todos los sistemas del motor ECS.
	*
	* Esta clase define la interfaz común para todos los sistemas del motor ECS.
	* Los sistemas son responsables de la lógica del juego y la gestión de recursos.
	* Cada sistema debe implementar la lógica específica en los métodos OnStart, OnUpdate y OnStop.
	* Los sistemas no deben acceder directamente a los datos de los componentes, sino a través de la Registry.
	*/

namespace ECS {

	//Forward declaration para evitar dependencias circulares con Registry
	class Registry;

	class
		System {
	public:
		virtual ~System() = default;

		//Inicialización del sistema (se llama una vez al registrarse)

		/*
		* @brief Se llama una vez al registrarse el sistema, ideal para inicializar recursos o referencias a componentes.
		*
		* @param registry Referencia al Registry del sistema.
		*/
		virtual void OnStart(Registry& registry) {}

		//Lógica frame-a-frame (se llama cada frame)
		/**
			* @brief Se llama cada frame, aquí va la lógica principal del sistema.
			*
			* @param registry Referencia al Registry del sistema.
			* @param deltaTime Tiempo transcurrido desde el último frame.
			*/
		virtual void OnUpdate(Registry& registry, float deltaTime) = 0;

		//Limpieza al destruir el sistema (se llama una vez al eliminarse)
		/**
			* @brief Se llama una vez al eliminar el sistema, ideal para limpiar recursos o referencias.
			*
			* @param registry Referencia al Registry del sistema.
			*/
		virtual void OnStop(Registry& registry) {}

		//Opcional: activa/desactiva el sistema sin eliminarlo (puede ser útil para sistemas de renderizado o lógica que no siempre debe ejecutarse)
		/**
			* @brief Activa o desactiva el sistema.
			*
			* @param enabled Estado deseado del sistema.
			*/
		void SetEnabled(bool enabled) noexcept { m_enabled = enabled; }

		/**
			* @brief Verifica si el sistema está habilitado.
			*
			* @return true si el sistema está habilitado, false en caso contrario.
			*/
		[[nodiscard]] bool IsEnabled() const { return m_enabled; }

	private:
		bool m_enabled = true;

	};

}
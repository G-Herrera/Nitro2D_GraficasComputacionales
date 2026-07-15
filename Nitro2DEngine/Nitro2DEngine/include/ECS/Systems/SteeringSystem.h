#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "ECS/Components/SteeringComponent.h"
#include "ECS/Components/Obstacle.h"
//=========================================================
// ECS::Systems/SteeringSystem.h
//
// LÓGICA de Steering Behaviors (Seek, Flee, Arrive).
// Recorre todas las entidades con
// (Transform, Velocity, Acceleration, SteeringComponent),
// calcula el vector de steering según los flags activos
// en SteeringComponent, lo limita por maxForce y lo escribe
// en Acceleration. Después integra Acceleration -> Velocity
// -> Transform.position respetando deltaTime.
//
// ORDEN: debe registrarse ANTES de CameraSystem, para que
// la cámara siga la posición ya actualizada en este mismo
// frame.
//=========================================================
namespace ECS {
	class Registry;

	//Snapshot de un obstáculo para este frame (evita re-consultar
	//la view de Obstacle por cada entidad con SteeringComponent)
	/**
		* @brief Estructura de datos que representa un obstáculo en el sistema de steering.
		*
		* @note Esta estructura se utiliza para almacenar información relevante sobre un obstáculo, 
		* incluyendo su ID de entidad, posición y radio. Se utiliza en el cálculo de la evasión de obstáculos en el sistema de steering.
		*/
	struct 
	ObstacleData {
		EntityID id;
		sf::Vector2f position;
		float radius;
	};

	class 
	SteeringSystem final : public System {
	public:
		SteeringSystem() = default;
		~SteeringSystem() = default;

		/**
			* @brief Se llama cada frame, aquí va la lógica principal del sistema.
			* 
			* @param registry Referencia al Registry del sistema.
			* @param deltaTime Tiempo transcurrido desde el último frame.
			* 
			* @note Este método recorre todas las entidades con (Transform, Velocity, Acceleration, SteeringComponent), 
			* calcula el vector de steering según los flags activos en SteeringComponent, lo limita por maxForce y lo escribe en Acceleration. 
			* Después integra Acceleration -> Velocity -> Transform.position respetando deltaTime.
			*/
		void
		OnUpdate(Registry& registry, float deltaTime) override;

	private:
		/**
			* @brief Calcula el vector de steering para Seek.
			* 
			* @param position Posición actual de la entidad.
			* @param velocity Velocidad actual de la entidad.
			* @param targetPos Posición objetivo a la que se desea llegar.
			* @param maxSpeed Velocidad máxima de la entidad.
			* 
			* @return sf::Vector2f Vector de steering calculado para Seek.
			* 
			* @note Este método calcula el vector de steering necesario para que la entidad busque 
			* alcanzar la posición objetivo (targetPos) desde su posición actual (position) y velocidad actual (velocity), 
			*	respetando la velocidad máxima (maxSpeed).
			*/
		[[nodiscard]] sf::Vector2f
			ComputeSeek(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const sf::Vector2f& targetPos,
				float maxSpeed) const noexcept;

		/*
			* @brief Calcula el vector de steering para Flee.
			* 
			* @param position Posición actual de la entidad.
			* @param velocity Velocidad actual de la entidad.
			* @param targetPos Posición objetivo de la que se desea huir.
			* @param maxSpeed Velocidad máxima de la entidad.
			* 
			* @return sf::Vector2f Vector de steering calculado para Flee.
			* 
			* @note Este método calcula el vector de steering necesario para que la entidad huya 
			*	de la posición objetivo (targetPos) desde su posición actual (position) y velocidad actual (velocity), 
			* respetando la velocidad máxima (maxSpeed).
			*/
		[[nodiscard]] sf::Vector2f
			ComputeFlee(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const sf::Vector2f& targetPos,
				float maxSpeed) const noexcept;

		/**
			* @brief Calcula el vector de steering para Arrive.
			* 
			* @param position Posición actual de la entidad.
			* @param velocity Velocidad actual de la entidad.
			* @param targetPos Posición objetivo a la que se desea llegar.
			* @param maxSpeed Velocidad máxima de la entidad.
			* @param slowingRadius Radio dentro del cual la entidad comenzará a desacelerar al acercarse al objetivo.
			* 
			* @return sf::Vector2f Vector de steering calculado para Arrive.
			* 
			* @note Este método calcula el vector de steering necesario para que la entidad llegue suavemente
			*/
		[[nodiscard]] sf::Vector2f
			ComputeArrive(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const sf::Vector2f& targetPos,
				float maxSpeed,
				float slowingRadius) const noexcept;

		/**
			* @brief Calcula el vector de steering para Wander.
			*
			* @param position Posición actual de la entidad.
			* @param velocity Velocidad actual de la entidad.
			* @param steering Componente de steering que almacena el estado interno del comportamiento
			*        (por ejemplo, el ángulo de wander).
			* @param deltaTime Tiempo transcurrido desde el último frame.
			*
			* @return sf::Vector2f Vector de steering calculado para Wander.
			*
			* @note Este comportamiento genera un movimiento aparentemente aleatorio pero suave,
			* manteniendo una dirección cambiante de forma continua.
			*/
		[[nodiscard]]
		sf::Vector2f ComputeWander(
			const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			SteeringComponent& steering,
			float deltaTime) const noexcept;

		/**
		  * @brief Calcula el vector de steering para Pursuit.
		  *
		  * @param position Posición actual de la entidad.
		  * @param velocity Velocidad actual de la entidad.
		  * @param targetPos Posición actual del objetivo.
		  * @param targetVelocity Velocidad actual del objetivo.
		  * @param maxSpeed Velocidad máxima de la entidad.
		  * @param predictionTime Tiempo utilizado para estimar la posición futura del objetivo.
		  *
		  * @return sf::Vector2f Vector de steering calculado para Pursuit.
		  *
		  * @note Este comportamiento predice la posición futura del objetivo en función de su
		  * velocidad y dirige a la entidad hacia ese punto, siendo más efectivo que un Seek
		  * cuando el objetivo está en movimiento.
		  */ 
		[[nodiscard]] sf::Vector2f
		ComputePursuit(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const sf::Vector2f& targetPos,
				const sf::Vector2f& targetVelocity,
				float maxSpeed,
				float predictionTime) const noexcept;

		/**
			* @brief Calcula el vector de steering para evitar obstáculos.
			*
			* @param position Posición actual de la entidad.
			* @param velocity Velocidad actual de la entidad.
			* @param obstacles Lista de obstáculos presentes en el entorno.
			* @param lookAhead Distancia hacia adelante utilizada para detectar posibles colisiones.
			* @param agentRadius Radio de la entidad utilizado para calcular la separación segura.
			* @param maxSpeed Velocidad máxima de la entidad.
			*
			* @return sf::Vector2f Vector de steering calculado para evitar el obstáculo más amenazante.
			*
			* @note El comportamiento busca el obstáculo que representa el mayor riesgo de colisión
			* dentro de la distancia de detección y genera una fuerza de evasión para esquivarlo.
			* Si no se detectan obstáculos peligrosos, devuelve un vector nulo.
			*/
		[[nodiscard]] sf::Vector2f
			ComputeObstacleAvoidance(EntityID selfId,
				const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const std::vector<ObstacleData>& obstacles,
				float lookAhead,
				float agentRadius,
				float maxSpeed) const noexcept;
	};
}
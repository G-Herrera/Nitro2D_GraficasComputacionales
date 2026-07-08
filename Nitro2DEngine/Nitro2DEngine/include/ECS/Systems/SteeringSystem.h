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
	//la view de Obstacle por cada entidad con SteeringComponent).
	struct ObstacleData {
		sf::Vector2f position;
		float radius;
	};

	class SteeringSystem final : public System
	{
	public:
		SteeringSystem() = default;
		~SteeringSystem() = default;

		void
			OnUpdate(Registry& registry, float deltaTime) override;

	private:
		//Calcula el vector de steering para Seek
		[[nodiscard]] sf::Vector2f
			ComputeSeek(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const sf::Vector2f& targetPos,
				float maxSpeed) const noexcept;

		//Calcula el vector de steering para Flee
		[[nodiscard]] sf::Vector2f
			ComputeFlee(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const sf::Vector2f& targetPos,
				float maxSpeed) const noexcept;

		//Calcula el vector de steering para Arrive (con frenado suave)
		[[nodiscard]] sf::Vector2f
			ComputeArrive(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const sf::Vector2f& targetPos,
				float maxSpeed,
				float slowingRadius) const noexcept;

		[[nodiscard]]
		sf::Vector2f ComputeWander(
			const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			SteeringComponent& steering,
			float deltaTime) const noexcept;

		//Calcula el vector de steering para Pursuit (Seek con predicción)
		[[nodiscard]] sf::Vector2f
			ComputePursuit(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const sf::Vector2f& targetPos,
				const sf::Vector2f& targetVelocity,
				float maxSpeed,
				float predictionTime) const noexcept;

		//Calcula el vector de steering para evitar el obstáculo más amenazante
		[[nodiscard]] sf::Vector2f
			ComputeObstacleAvoidance(const sf::Vector2f& position,
				const sf::Vector2f& velocity,
				const std::vector<ObstacleData>& obstacles,
				float lookAhead,
				float agentRadius,
				float maxSpeed) const noexcept;
	};
}
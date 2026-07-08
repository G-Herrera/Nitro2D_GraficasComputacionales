#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"
#include "ECS/Components/Obstacle.h"
#include "Modules/Math2D.h"
#include <cmath>
#include <random>

namespace
{
	std::mt19937 g_rng(std::random_device{}());

	std::uniform_real_distribution<float> g_random(-1.f, 1.f);
}

namespace ECS {

	sf::Vector2f
		SteeringSystem::ComputeSeek(const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const sf::Vector2f& targetPos,
			float maxSpeed) const noexcept {
		sf::Vector2f desired = Math::Normalize(targetPos - position) * maxSpeed;
		
		return desired - velocity; // steering
	}

	sf::Vector2f
		SteeringSystem::ComputeFlee(const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const sf::Vector2f& targetPos,
			float maxSpeed) const noexcept {
		sf::Vector2f desired = Math::Normalize(position - targetPos) * maxSpeed;

		return desired - velocity; // steering
	}

	sf::Vector2f
		SteeringSystem::ComputeArrive(const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const sf::Vector2f& targetPos,
			float maxSpeed,
			float slowingRadius) const noexcept {
		sf::Vector2f desired = targetPos - position;
		const float distance = Math::Length(desired);

		if (distance <= 0.f) return {};

		float speed = maxSpeed;

		if (distance < slowingRadius)
		{
			speed *= distance / slowingRadius;
		}

		desired = Math::Normalize(desired) * speed;

		return desired - velocity; // steering
	}

	sf::Vector2f
		SteeringSystem::ComputeWander(
			const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			SteeringComponent& steering,
			float deltaTime) const noexcept
	{
		steering.wanderAngle += g_random(g_rng) * steering.wanderJitter * deltaTime;

		sf::Vector2f forward = Math::Normalize(velocity);

		if (Math::LengthSquared(forward) == 0.f)
		{
			forward = { 1.f, 0.f };
		}

		sf::Vector2f circleCenter = forward * steering.wanderDistance;

		sf::Vector2f displacement;
		displacement.x = std::cos(steering.wanderAngle);
		displacement.y = std::sin(steering.wanderAngle);
		displacement *= steering.wanderRadius;

		sf::Vector2f wanderTarget = position + circleCenter + displacement;


		return ComputeSeek(position, velocity, wanderTarget, steering.maxSpeed);
	}

	sf::Vector2f
		SteeringSystem::ComputePursuit(const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const sf::Vector2f& targetPos,
			const sf::Vector2f& targetVelocity,
			float maxSpeed,
			float predictionTime) const noexcept {
		// Predice dónde estará el target y persigue ese punto (Seek clásico)
		const sf::Vector2f predictedPos = targetPos + targetVelocity * predictionTime;
		return ComputeSeek(position, velocity, predictedPos, maxSpeed);
	}

	sf::Vector2f
		SteeringSystem::ComputeObstacleAvoidance(const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const std::vector<ObstacleData>& obstacles,
			float lookAhead,
			float agentRadius,
			float maxSpeed) const noexcept {

		sf::Vector2f dir = Math::Normalize(velocity);
		if (Math::LengthSquared(dir) == 0.f) return {};

		//Dos puntos de "mirada" adelante: uno lejano y uno cercano
		const sf::Vector2f ahead = position + dir * lookAhead;
		const sf::Vector2f ahead2 = position + dir * (lookAhead * 0.5f);

		const ObstacleData* mostThreatening = nullptr;
		float closestDist = 0.f;

		for (const auto& obs : obstacles) {
			const float combinedRadius = obs.radius + agentRadius;
			const bool collision =
				Math::Distance(obs.position, ahead) <= combinedRadius ||
				Math::Distance(obs.position, ahead2) <= combinedRadius ||
				Math::Distance(obs.position, position) <= combinedRadius;

			if (collision) {
				const float dist = Math::Distance(position, obs.position);
				if (!mostThreatening || dist < closestDist) {
					mostThreatening = &obs;
					closestDist = dist;
				}
			}
		}

		if (!mostThreatening) return {};

		//Fuerza perpendicular que aleja al agente del obstáculo más amenazante
		sf::Vector2f avoidance = ahead - mostThreatening->position;
		return Math::Normalize(avoidance) * maxSpeed;
	}

	void
		SteeringSystem::OnUpdate(Registry& registry, float deltaTime) {

		// Obstáculos del frame (se calculan una sola vez, no por entidad)
		std::vector<ObstacleData> obstacles;
		registry.GetView<Transform, Obstacle>().Each(
			[&obstacles](EntityID, Transform& t, Obstacle& o) {
				obstacles.push_back({ t.position, o.radius });
			});

		registry.GetView<Transform, Velocity, Acceleration, SteeringComponent>().Each(
			[this, &registry, deltaTime, &obstacles](EntityID, Transform& transform, Velocity& vel,
				Acceleration& accel, SteeringComponent& steer) {

					sf::Vector2f steeringForce{ 0.f, 0.f };

					if (steer.target != NULL_ENTITY && registry.IsAlive(steer.target)) {
						if (auto* targetTransform = registry.TryGetComponent<Transform>(steer.target)) {

							if (steer.seekEnabled) {
								steeringForce += ComputeSeek(transform.position, vel.velocity,
									targetTransform->position, steer.maxSpeed);
							}
							if (steer.fleeEnabled) {
								steeringForce += ComputeFlee(transform.position, vel.velocity,
									targetTransform->position, steer.maxSpeed);
							}
							if (steer.arriveEnabled) {
								steeringForce += ComputeArrive(transform.position, vel.velocity,
									targetTransform->position, steer.maxSpeed,
									steer.slowingRadius);
							}
							if (steer.pursuitEnabled) {
								sf::Vector2f targetVel{ 0.f, 0.f };
								if (auto* targetVelocity = registry.TryGetComponent<Velocity>(steer.target)) {
									targetVel = targetVelocity->velocity;
								}
								steeringForce += ComputePursuit(transform.position, vel.velocity,
									targetTransform->position, targetVel,
									steer.maxSpeed, steer.pursuitPredictionTime);
							}
						}
					}

					if (steer.wanderEnabled) {
						steeringForce += ComputeWander(transform.position, vel.velocity,
							steer, deltaTime);
					}

					if (steer.obstacleAvoidanceEnabled) {
						steeringForce += ComputeObstacleAvoidance(transform.position, vel.velocity,
							obstacles, steer.obstacleLookAhead, steer.obstacleRadius, steer.maxSpeed);
					}

					//Limitar la fuerza de steering resultante
					steeringForce = Math::Truncate(steeringForce, steer.maxForce);
					accel.acceleration = steeringForce;

					//Integración física: acceleration -> velocity -> position
					vel.velocity += accel.acceleration * deltaTime;
					vel.velocity = Math::Truncate(vel.velocity, steer.maxSpeed);

					transform.position += vel.velocity * deltaTime;
			});
	}
}
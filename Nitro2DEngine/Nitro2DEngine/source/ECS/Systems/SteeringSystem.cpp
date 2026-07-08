#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"
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

		return {};
	}

	void
		SteeringSystem::OnUpdate(Registry& registry, float deltaTime) {
		registry.GetView<Transform, Velocity, Acceleration, SteeringComponent>().Each(
			[this, &registry, deltaTime](EntityID, Transform& transform, Velocity& vel,
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
						}
					}

					//Limitar la fuerza de steering resultante
					steeringForce = Math::Truncate(steeringForce,steer.maxForce);
					accel.acceleration = steeringForce;

					//Integración física: acceleration -> velocity -> position
					vel.velocity += accel.acceleration * deltaTime;
					vel.velocity = Math::Truncate(vel.velocity, steer.maxSpeed);

					transform.position += vel.velocity * deltaTime;
			});
	}
}
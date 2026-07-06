#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"
#include <cmath>

namespace ECS {

	sf::Vector2f
		SteeringSystem::Limit(const sf::Vector2f& vector, float max) const noexcept {
		const float lengthSq = vector.x * vector.x + vector.y * vector.y;
		if (lengthSq > max * max && lengthSq > 0.f) {
			const float length = std::sqrt(lengthSq);
			return vector * (max / length);
		}
		return vector;
	}

	sf::Vector2f
		SteeringSystem::ComputeSeek(const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const sf::Vector2f& targetPos,
			float maxSpeed) const noexcept {
		sf::Vector2f desired = targetPos - position;
		const float length = std::sqrt(desired.x * desired.x + desired.y * desired.y);
		if (length > 0.f) {
			desired = desired * (maxSpeed / length); // normalize * maxSpeed
		}
		return desired - velocity; // steering
	}

	sf::Vector2f
		SteeringSystem::ComputeFlee(const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const sf::Vector2f& targetPos,
			float maxSpeed) const noexcept {
		sf::Vector2f desired = position - targetPos;
		const float length = std::sqrt(desired.x * desired.x + desired.y * desired.y);
		if (length > 0.f) {
			desired = desired * (maxSpeed / length); // normalize * maxSpeed
		}
		return desired - velocity; // steering
	}

	sf::Vector2f
		SteeringSystem::ComputeArrive(const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const sf::Vector2f& targetPos,
			float maxSpeed,
			float slowingRadius) const noexcept {
		sf::Vector2f desired = targetPos - position;
		const float distance = std::sqrt(desired.x * desired.x + desired.y * desired.y);

		if (distance > 0.f) {
			float speed = maxSpeed;
			if (slowingRadius > 0.f && distance < slowingRadius) {
				// Dentro del radio: frenado proporcional a la distancia
				speed = maxSpeed * (distance / slowingRadius);
			}
			desired = desired * (speed / distance); // normalize * speed
		}

		return desired - velocity; // steering
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
					steeringForce = Limit(steeringForce, steer.maxForce);
					accel.acceleration = steeringForce;

					//Integración física: acceleration -> velocity -> position
					vel.velocity += accel.acceleration * deltaTime;
					vel.velocity = Limit(vel.velocity, steer.maxSpeed);

					transform.position += vel.velocity * deltaTime;
			});
	}
}
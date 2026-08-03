#include "ECS/Systems/SteeringSystem.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Velocity.h"
#include "ECS/Components/Acceleration.h"
#include "ECS/Components/SteeringComponent.h"
#include "ECS/Components/Obstacle.h"
#include "Modules/Math2D.h"
#include "ECS/Components/PathComponent.h"
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

	PathFollowingResult
		SteeringSystem::ComputePathFollowing(
			const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const PathComponent& path,
			float maxSpeed,
			float aheadDistance) const noexcept
	{
		PathFollowingResult result;

		// Se necesitan al menos dos puntos para formar un segmento.
		if (path.points.size() < 2)
		{
			return result;
		}

		// Dirección actual del agente.
		sf::Vector2f forward =
			Math::Normalize(velocity);

		// Dirección temporal para un agente detenido.
		if (Math::LengthSquared(forward) <= 0.00001f)
		{
			forward = { 1.f, 0.f };
		}

		// Predecir la posición futura del kart.
		const float predictionDistance =
			std::max(20.f, aheadDistance * 0.5f);

		result.predictedPosition =
			position + forward * predictionDistance;

		// Encontrar la proyección de la posición futura
		// sobre la línea central del path.
		const Math::NearestPathResult nearest =
			Math::NearestPointOnPath(
				path.points,
				path.closed,
				result.predictedPosition);

		result.nearestPoint =
			nearest.point;

		// Avanzar sobre la línea central para encontrar
		// el punto que el agente perseguirá.
		result.targetPoint =
			Math::PointAheadOnPath(
				path.points,
				path.closed,
				nearest.segmentIndex,
				nearest.point,
				aheadDistance);

		// Generar la fuerza continua hacia el objetivo adelantado.
		result.force =
			ComputeSeek(
				position,
				velocity,
				result.targetPoint,
				maxSpeed);

		return result;
	}

	sf::Vector2f
	SteeringSystem::ComputeObstacleAvoidance(EntityID selfId,
			const sf::Vector2f& position,
			const sf::Vector2f& velocity,
			const std::vector<ObstacleData>& obstacles,
			float lookAhead,
			float agentRadius,
			float maxSpeed) const noexcept {

		sf::Vector2f dir = Math::Normalize(velocity);
		if (Math::LengthSquared(dir) == 0.f) return {};

		const sf::Vector2f ahead = position + dir * lookAhead;
		const sf::Vector2f ahead2 = position + dir * (lookAhead * 0.5f);

		const ObstacleData* mostThreatening = nullptr;
		float closestDist = 0.f;

		for (const auto& obs : obstacles) {
			if (obs.id == selfId) continue; // no evitarse a si mismo

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

		sf::Vector2f avoidance = ahead - mostThreatening->position;
		return Math::Normalize(avoidance) * maxSpeed;
	}

	void
	SteeringSystem::OnUpdate(Registry& registry, float deltaTime) {

		// Obstáculos del frame: TODA entidad con (Transform, Obstacle),
		// sin importar el target de nadie. Se calcula una sola vez.
		std::vector<ObstacleData> obstacles;
		registry.GetView<Transform, Obstacle>().Each(
			[&obstacles](EntityID id, Transform& t, Obstacle& o) {
				obstacles.push_back({ id, t.position, o.radius });
			});

		registry.GetView<Transform, Velocity, Acceleration, SteeringComponent>().Each(
			[this, &registry, deltaTime, &obstacles](EntityID entity, Transform& transform, Velocity& vel,
				Acceleration& accel, SteeringComponent& steer) {

					SteeringDebugComponent* debug = registry.TryGetComponent<SteeringDebugComponent>(entity);

					const bool anyBehaviorEnabled =
						steer.seekEnabled || steer.fleeEnabled || steer.arriveEnabled ||
						steer.pursuitEnabled || steer.wanderEnabled || steer.obstacleAvoidanceEnabled || 
						steer.pathFollowingEnabled;

					if (!anyBehaviorEnabled)
					{
						accel.acceleration = { 0.f, 0.f };

						vel.velocity = { 0.f, 0.f };

						if (auto* debug = registry.TryGetComponent<SteeringDebugComponent>(entity))
						{
							debug->velocity = { 0.f, 0.f };

							debug->pathFollowingForce = { 0.f, 0.f };

							debug->separationForce = { 0.f, 0.f };

							debug->finalSteeringForce = { 0.f, 0.f };
						}

						return; // sin comportamiento activo: la entidad no se mueve este frame
					} 

					// --- Steering por prioridades (weighted truncated running sum) ---
					// Cada comportamiento se recorta al presupuesto de fuerza que
					// aún queda antes de sumarse. Así, un comportamiento de alta
					// prioridad (Obstacle Avoidance) SIEMPRE se aplica al completo
					// antes de que los demás compitan por lo que sobra, en vez de
					// sumarse todo junto y arriesgarse a diluirse en el truncado final.
					sf::Vector2f steeringForce{ 0.f, 0.f };
					float forceBudget = steer.maxForce;

					auto accumulate = [&](const sf::Vector2f& force) {
						if (forceBudget <= 0.f) return;
						const sf::Vector2f clipped = Math::Truncate(force, forceBudget);
						steeringForce += clipped;
						forceBudget -= Math::Length(clipped);
						};

					// Prioridad 1 (más alta): evitar obstáculos. Independiente de target.
					if (steer.obstacleAvoidanceEnabled) {
						accumulate(ComputeObstacleAvoidance(entity, transform.position, vel.velocity,
							obstacles, steer.obstacleLookAhead, steer.obstacleRadius, steer.maxSpeed));
					}

					// Prioridad 2: seguir o recuperar el camino.
					// No utiliza SteeringComponent::target; utiliza pathEntity.
					if (steer.pathFollowingEnabled &&
						steer.pathEntity != NULL_ENTITY &&
						registry.IsAlive(steer.pathEntity))
					{
						if (auto* path =
							registry.TryGetComponent<PathComponent>(
								steer.pathEntity))
						{
							const PathFollowingResult pathResult =
								ComputePathFollowing(
									transform.position,
									vel.velocity,
									*path,
									steer.maxSpeed,
									steer.pathAheadDistance);

							accumulate(pathResult.force);

							// Escribir la información geométrica únicamente si
							// la entidad posee SteeringDebugComponent.
							if (auto* debug =
								registry.TryGetComponent<SteeringDebugComponent>(
									entity))
							{
								debug->predictedPosition =
									pathResult.predictedPosition;

								debug->nearestPathPoint =
									pathResult.nearestPoint;

								debug->pathTargetPoint =
									pathResult.targetPoint;

								debug->pathFollowingForce =
									pathResult.force;
							}
						}
					}

					// Prioridad 3: comportamientos que dependen de target
					if (steer.target != NULL_ENTITY && registry.IsAlive(steer.target)) {
						if (auto* targetTransform = registry.TryGetComponent<Transform>(steer.target)) {

							if (steer.fleeEnabled) {
								accumulate(ComputeFlee(transform.position, vel.velocity,
									targetTransform->position, steer.maxSpeed));
							}
							if (steer.arriveEnabled) {
								accumulate(ComputeArrive(transform.position, vel.velocity,
									targetTransform->position, steer.maxSpeed, steer.slowingRadius));
							}
							if (steer.pursuitEnabled) {
								sf::Vector2f targetVel{ 0.f, 0.f };
								if (auto* targetVelocity = registry.TryGetComponent<Velocity>(steer.target)) {
									targetVel = targetVelocity->velocity;
								}
								accumulate(ComputePursuit(transform.position, vel.velocity,
									targetTransform->position, targetVel,
									steer.maxSpeed, steer.pursuitPredictionTime));
							}
							if (steer.seekEnabled) {
								accumulate(ComputeSeek(transform.position, vel.velocity,
									targetTransform->position, steer.maxSpeed));
							}
						}
					}

					// Prioridad 4 (más baja): Wander, es solo relleno cosmético
					if (steer.wanderEnabled) {
						accumulate(ComputeWander(transform.position, vel.velocity, steer, deltaTime));
					}

					accel.acceleration = steeringForce;

					// Guardar la fuerza final ANTES de integrar el movimiento.
					if (auto* debug = registry.TryGetComponent<SteeringDebugComponent>(entity))
					{
						debug->finalSteeringForce = steeringForce;
					}

					// Integración física: acceleration -> velocity -> position.
					vel.velocity += accel.acceleration * deltaTime;
					vel.velocity = Math::Truncate(vel.velocity,steer.maxSpeed);
					transform.position += vel.velocity * deltaTime;

					// Guardar la velocidad final del frame.
					if (auto* debug = registry.TryGetComponent<SteeringDebugComponent>(entity))
					{
						debug->velocity = vel.velocity;
					}
			});
	}
}
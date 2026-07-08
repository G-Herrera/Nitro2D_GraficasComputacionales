#pragma once
// ======================================================
// ECS :: Components/Obstacle.h
//
// Obstacle - componente de DATOS PUROS (sin lógica).
// Marca una entidad como obstáculo para el steering
// behavior "Obstacle Avoidance" y define su radio de
// colisión (círculo).
// ======================================================
namespace ECS {
	struct Obstacle {
		float radius{ 20.f };

		Obstacle() = default;
		explicit Obstacle(float r) noexcept : radius(r) {}
	};
}
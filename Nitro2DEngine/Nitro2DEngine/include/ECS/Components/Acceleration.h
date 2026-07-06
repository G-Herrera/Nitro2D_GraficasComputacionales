#pragma once
#include "Prerequisites.h"
// ======================================================
// ECS :: Components/Acceleration.h
//
// Acceleration - componente de DATOS PUROS (sin lógica).
// Describe la aceleración lineal actual de una entidad.
// Es el resultado de aplicar una fuerza de steering (u otra
// fuerza) sobre la entidad; el SteeringSystem escribe aquí
// y el propio SteeringSystem (o un futuro MovementSystem)
// la integra sobre Velocity.
//
// Como todo componente de este ECS, es un simple struct:
// - Se almacena en un ComponentPool<Acceleration>
// - No hereda de nada ni tiene métodos
// ======================================================
namespace ECS {
	struct Acceleration {
		//Aceleración en pixels/segundo^2.
		sf::Vector2f acceleration{ 0.f, 0.f };

		//Constructores de conveniencia
		Acceleration() = default;
		explicit Acceleration(sf::Vector2f accel) noexcept : acceleration(accel) {}
	};
}
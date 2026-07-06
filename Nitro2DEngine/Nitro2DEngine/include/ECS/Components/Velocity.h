#pragma once
#include "Prerequisites.h"
// ======================================================
// ECS :: Components/Velocity.h
//
// Velocity - componente de DATOS PUROS (sin lógica).
// Describe la velocidad lineal actual de una entidad,
// en pixels por segundo.
//
// Como todo componente de este ECS, es un simple struct:
// - Se almacena en un ComponentPool<Velocity>
// - No hereda de nada ni tiene métodos
// ======================================================
namespace ECS {
	struct Velocity {
		//Velocidad en pixels/segundo (coordenadas de mundo).
		sf::Vector2f velocity{ 0.f, 0.f };

		//Constructores de conveniencia
		Velocity() = default;
		explicit Velocity(sf::Vector2f vel) noexcept : velocity(vel) {}
	};
}
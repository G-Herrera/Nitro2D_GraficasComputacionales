#pragma once
#include "Prerequisites.h"

// ======================================================
// ECS :: Components/PathComponent.h
//
// PathComponent - componente de DATOS PUROS (sin logica).
// Representa un camino cerrado (circuito) como una polilinea
// DENSA ya muestreada (ver Math::BuildClosedCatmullRom), lista
// para que SteeringSystem haga geometria de segmento de linea
// (proyeccion, punto mas cercano, punto adelante) sin tener que
// evaluar una curva en tiempo real.
//
// Se agrega a UNA entidad que representa el circuito (no a cada
// kart). Los karts solo guardan una referencia (EntityID) a esa
// entidad, igual que ya hace SteeringComponent::target.
// ======================================================
namespace ECS {
	struct PathComponent {
		std::vector<sf::Vector2f> points; // polilinea densa, ya muestreada
		bool closed{ true };              // true = circuito cerrado (vuelve al inicio)
		float radius{ 40.f };             // ancho de tolerancia del camino (mitad del ancho transitable)

		PathComponent() = default;
	};
}
#pragma once
#include "ECS/Types.h"
// ======================================================
// ECS :: Components/SteeringComponent.h
//
// SteeringComponent - componente de DATOS PUROS (sin lógica).
// Activa/desactiva los comportamientos de Steering (Seek,
// Flee, Arrive) y guarda los parámetros que el
// SteeringSystem necesita para calcularlos.
//
// El comportamiento en sí (las fórmulas vectoriales) vive
// exclusivamente en SteeringSystem; este struct solo
// almacena el "qué" y el "con qué parámetros", nunca el
// "cómo".
//
// Como todo componente de este ECS, es un simple struct:
// - Se almacena en un ComponentPool<SteeringComponent>
// - No hereda de nada ni tiene métodos
// ======================================================
namespace ECS {
	struct SteeringComponent {
		//Comportamientos activos (pueden combinarse; el SteeringSystem
		//suma sus contribuciones y limita el resultado por maxForce).
		bool seekEnabled{ false };
		bool fleeEnabled{ false };
		bool arriveEnabled{ false };

		//Entidad objetivo (debe tener Transform). NULL_ENTITY = sin objetivo.
		EntityID target{ NULL_ENTITY };

		//Límites físicos del steering
		float maxSpeed{ 100.f };
		float maxForce{ 50.f };

		//Radio de frenado, usado únicamente por Arrive
		float slowingRadius{ 100.f };

		SteeringComponent() = default;
	};
}
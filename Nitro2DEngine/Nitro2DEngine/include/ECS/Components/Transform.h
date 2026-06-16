#pragma once
#include "Prerequisites.h"

// ======================================================
// ECS :: Components/Transform.h
//
// Transform - componente de DATOS PUROS (sin lógica).
// Describe la posición, rotación y escala de una entidad
// en el espacio 2D del motor.
//
// Como todo componenet de este ECS, es un simple struct:
// - Se almacena en un ComponentPool<Transform>
// - No hereda de nada ni tiene métodos
// ======================================================

namespace ECS {

	struct Transform {
		//Posición en pixels (coordenadas de mundo).
		sf::Vector2f position{ 0.f, 0.f };

		//Rotación en grados (SFML usa grados, sentido horario).
		float rotation{ 0.f };

		//Escala por eje (1,1 = tamaño original)
		sf::Vector2f scale{ 1.f, 1.f };

		//Constructores de conveniencia
		Transform() = default;

		explicit Transform(sf::Vector2f pos, float rot = 0.f, sf::Vector2f scl = {1.f, 1.f}) 
											noexcept : position(pos), rotation(rot), scale(scl){}

		void Translate(sf::Vector2f delta) noexcept { position += delta; }
	
		void Rotate(float degrees) noexcept { rotation += degrees; }
	};
}
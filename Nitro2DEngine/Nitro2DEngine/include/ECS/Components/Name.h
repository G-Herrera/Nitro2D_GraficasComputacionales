#pragma once
#include <string>
// ======================================================
// ECS :: Components/Name.h
//
// Name - componente de DATOS PUROS (sin lógica).
// Etiqueta legible para una entidad, usada por el editor
// (Hierarchy, combos de selección de target, etc.) en vez
// de mostrar el EntityID crudo.
// ======================================================
namespace ECS {
	struct Name {
		std::string name{ "Entity" };

		Name() = default;
		explicit Name(std::string n) noexcept : name(std::move(n)) {}
	};
}
#pragma once
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "Core/Window.h"

// =====
// ECS :: Systems/RenderSystme.h
// 
// LÓGICA pura: recorre todas las entidades que tengan a la vez
// Tranform + Render, vuelca el Transform sobre sf::Shape
// y la dibuja en la venta.
// 
// Debe llamarse DENTRO del frame, entre Window::clear()
// y Window::display()
// =====

namespace ECS {

	class 
	RenderSystem final : public System{
	public:
		explicit RenderSystem(Window& window) noexcept : m_window(window){}

		/**
			* @brief Se llama cada frame, aquí va la lógica principal del sistema.
			* 
			* @param registry Referencia al Registry del sistema.
			* @param deltaTime Tiempo transcurrido desde el último frame.
			* 
			* @note Este método recorre todas las entidades que tengan a la vez Transform + Render, 
			*	vuelca el Transform sobre sf::Shape y la dibuja en la ventana.
			*/
		void
		OnUpdate(Registry& registry, float/*deltaTime*/) override {

			// View::Each itera en orden INVERSO al de inserción (a propósito,
			// para permitir eliminar entidades de forma segura durante la
			// iteración). Ese orden no es apto para dibujar, así que aquí
			// recolectamos primero y luego ordenamos explícitamente por
			// Render::zOrder antes de dibujar.
			m_drawList.clear();

			registry.GetView<Transform, Render>().Each(
				[this](EntityID, const Transform& t, const Render& r) {
					if (!r.shape || !r.visible) return;
					m_drawList.push_back({ &t, &r });
				});

			std::stable_sort(m_drawList.begin(), m_drawList.end(),
				[](const DrawEntry& a, const DrawEntry& b) {
					return a.render->zOrder < b.render->zOrder;
				});

			for (const auto& entry : m_drawList) {
				entry.render->shape->setPosition(entry.transform->position);
				entry.render->shape->setRotation(sf::degrees(entry.transform->rotation));
				entry.render->shape->setScale(entry.transform->scale);
				entry.render->shape->setFillColor(entry.render->fillColor);
				m_window.draw(*entry.render->shape);
			}
		}
	private:
		struct DrawEntry {
			const Transform* transform;
			const Render* render;
		};
		std::vector<DrawEntry> m_drawList; ///< Buffer reutilizado cada frame para evitar reasignar memoria constantemente.
		Window& m_window;///< Referencia a la ventana donde se dibujarán las entidades con componentes Render.
	};

}
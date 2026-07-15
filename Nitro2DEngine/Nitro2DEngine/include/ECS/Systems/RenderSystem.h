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

				registry.GetView<Transform, Render>().Each([this](EntityID /*entity*/, const Transform& t, const Render& r) {
				if (!r.shape || !r.visible) return;

				//Vuelca el estado del transform sobre la forma sfml
				r.shape->setPosition(t.position);
				r.shape->setRotation(sf::degrees(t.rotation));
				r.shape->setScale(t.scale);
				r.shape->setFillColor(r.fillColor);

				m_window.draw(*r.shape);
				});

		}
	private:
		Window& m_window;///< Referencia a la ventana donde se dibujarán las entidades con componentes Render.
	};

}
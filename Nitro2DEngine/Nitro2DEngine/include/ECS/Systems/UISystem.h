#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/Camera.h"

namespace ECS {
	class UISystem final : public System

	{
	public:
		UISystem() = default;

		void 
		OnUpdate(Registry& registry, float deltaTime) override {
      Outliner(registry);
      Inspector(registry);  
		}

		void 
		Outliner(Registry& registry) {
			ImGui::Begin("Hierarchy");

			for (auto entity : registry.GetAllEntities()) {
				if (!registry.IsAlive(entity)) continue;

				std::string label = "Entity " + std::to_string(entity);

				if (ImGui::Selectable(label.c_str(), selectedEntity == entity))
				{
					selectedEntity = entity;
				}
			}

      ImGui::End();
		}

		void
		Inspector(Registry& registry) {
      ImGui::Begin("Inspector");
      if (registry.IsAlive(selectedEntity))
      {
        auto& transform = registry.GetComponent<ECS::Transform>(selectedEntity);

        ImGui::Text("Transform");

        ImGui::DragFloat2("Position", (float*)&transform.position, 1.0f);
        ImGui::DragFloat("Rotation", &transform.rotation, 1.0f);
        ImGui::DragFloat2("Scale", (float*)&transform.scale, 0.01f);
      }
      else
      {
        ImGui::Text("No hay entidad seleccionada");
      }

      if (registry.HasComponent<ECS::Render>(selectedEntity))
      {
        auto& render = registry.GetComponent<ECS::Render>(selectedEntity);

        ImGui::Separator();
        ImGui::Text("Render");

        float color[4] = {
            render.fillColor.r / 255.f,
            render.fillColor.g / 255.f,
            render.fillColor.b / 255.f,
            render.fillColor.a / 255.f
        };

        if (ImGui::ColorEdit4("Color", color))
        {
          render.fillColor = sf::Color(
            static_cast<uint8_t>(color[0] * 255),
            static_cast<uint8_t>(color[1] * 255),
            static_cast<uint8_t>(color[2] * 255),
            static_cast<uint8_t>(color[3] * 255)
          );

          if (render.shape)
            render.shape->setFillColor(render.fillColor);
        }

        if (auto* cam = registry.TryGetComponent<ECS::Camera>(selectedEntity)) {
          if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
            // La posición y la rotación de la cámara se editan arriba,
            // en el bloque Transform. Aquí solo lo propio de la cámara.
            ImGui::Checkbox("Active", &cam->active);
            ImGui::DragFloat("Zoom", &cam->zoom, 0.01f, 0.05f, 10.f);
            ImGui::DragFloat("Follow Speed", &cam->followSpeed, 0.1f, 0.f, 50.f);

            // Objetivo a seguir (solo lectura aquí; muestra el ID).
            if (cam->followTarget == ECS::NULL_ENTITY)
              ImGui::Text("Follow Target: (ninguno)");
            else
              ImGui::Text("Follow Target: %llu",
                static_cast<unsigned long long>(cam->followTarget));
          }
        }
      }
      ImGui::End();
		}


	private:
		ECS::EntityID selectedEntity = ECS::NULL_ENTITY;
	};

};

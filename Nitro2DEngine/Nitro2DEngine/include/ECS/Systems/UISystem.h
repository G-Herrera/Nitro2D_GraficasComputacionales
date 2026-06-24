#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"

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
      }
      ImGui::End();
		}


	private:
		ECS::EntityID selectedEntity = ECS::NULL_ENTITY;
	};

};

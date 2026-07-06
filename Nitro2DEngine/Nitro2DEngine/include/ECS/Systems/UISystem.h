#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/SteeringComponent.h"

namespace ECS {
	class UISystem final : public System

	{
	public:
		UISystem() = default;

    void Initialize()
    {
      ApplyStyle();
    }

    void 
    ApplyStyle()
    {
      ImGuiStyle& style = ImGui::GetStyle();

      style.WindowRounding = 2.f;
      style.FrameRounding = 2.f;
      style.TabRounding = 2.f;
      style.GrabRounding = 2.f;

      style.WindowBorderSize = 1.f;
      style.FrameBorderSize = 1.f;

      ImVec4* colors = style.Colors;

      //=========================================================
      // Texto
      //=========================================================

      colors[ImGuiCol_Text] = ImVec4(0.93f, 0.97f, 0.95f, 1.f);
      colors[ImGuiCol_TextDisabled] = ImVec4(0.55f, 0.63f, 0.60f, 1.f);

      //=========================================================
      // Fondo
      //=========================================================

      // #29332F
      colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.20f, 0.18f, 1.f);

      // ligeramente más oscuro
      colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.17f, 0.15f, 1.f);

      colors[ImGuiCol_PopupBg] = ImVec4(0.16f, 0.20f, 0.18f, 1.f);

      //=========================================================
      // Bordes
      //=========================================================

      colors[ImGuiCol_Border] = ImVec4(0.21f, 0.53f, 0.41f, 0.65f);

      //=========================================================
      // Frames
      //=========================================================

      // #325448
      colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.33f, 0.28f, 1.f);

      // #368769
      colors[ImGuiCol_FrameBgHovered] = ImVec4(0.21f, 0.53f, 0.41f, 1.f);

      // #25BA84
      colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.73f, 0.52f, 1.f);

      //=========================================================
      // Botones
      //=========================================================

      colors[ImGuiCol_Button] = ImVec4(0.20f, 0.33f, 0.28f, 1.f);
      colors[ImGuiCol_ButtonHovered] = ImVec4(0.15f, 0.73f, 0.52f, 1.f);
      colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);

      //=========================================================
      // Headers
      //=========================================================

      colors[ImGuiCol_Header] = ImVec4(0.20f, 0.33f, 0.28f, 1.f);
      colors[ImGuiCol_HeaderHovered] = ImVec4(0.15f, 0.73f, 0.52f, 1.f);
      colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);

      //=========================================================
      // Checkboxes / Sliders
      //=========================================================

      colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);

      colors[ImGuiCol_SliderGrab] = ImVec4(0.15f, 0.73f, 0.52f, 1.f);
      colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);

      //=========================================================
      // Barra de título
      //=========================================================

      colors[ImGuiCol_TitleBg] = ImVec4(0.20f, 0.33f, 0.28f, 1.f);
      colors[ImGuiCol_TitleBgActive] = ImVec4(0.21f, 0.53f, 0.41f, 1.f);
      colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.16f, 0.20f, 0.18f, 1.f);

      //=========================================================
      // Tabs
      //=========================================================

      colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.33f, 0.28f, 1.f);
      colors[ImGuiCol_TabHovered] = ImVec4(0.15f, 0.73f, 0.52f, 1.f);
      colors[ImGuiCol_TabActive] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);

      colors[ImGuiCol_TabUnfocused] = ImVec4(0.18f, 0.24f, 0.21f, 1.f);
      colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.21f, 0.53f, 0.41f, 1.f);

      //=========================================================
      // Menú
      //=========================================================

      colors[ImGuiCol_MenuBarBg] = ImVec4(0.16f, 0.20f, 0.18f, 1.f);

      //=========================================================
      // Separadores
      //=========================================================

      colors[ImGuiCol_Separator] = ImVec4(0.21f, 0.53f, 0.41f, 0.50f);
      colors[ImGuiCol_SeparatorHovered] = ImVec4(0.15f, 0.73f, 0.52f, 1.f);
      colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);

      //=========================================================
      // Resize
      //=========================================================

      colors[ImGuiCol_ResizeGrip] = ImVec4(0.21f, 0.53f, 0.41f, 0.45f);
      colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.15f, 0.73f, 0.52f, 1.f);
      colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);

      //=========================================================
      // Docking
      //=========================================================

      colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.93f, 0.59f, 0.55f);
      colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.16f, 0.20f, 0.18f, 1.f);

      //=========================================================
      // Scrollbar
      //=========================================================

      colors[ImGuiCol_ScrollbarBg] = ImVec4(0.13f, 0.17f, 0.15f, 1.f);
      colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.20f, 0.33f, 0.28f, 1.f);
      colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.21f, 0.53f, 0.41f, 1.f);
      colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);

      //=========================================================
      // Selección de texto
      //=========================================================

      colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 0.93f, 0.59f, 0.35f);

      colors[ImGuiCol_NavHighlight] = ImVec4(0.00f, 0.93f, 0.59f, 1.f);
    }

		void 
		OnUpdate(Registry& registry, float deltaTime) override {
      if (!m_initialized)
      {
        ApplyStyle();
        m_initialized = true;
      }

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

        if (registry.HasComponent<ECS::SteeringComponent>(selectedEntity))
        {
          auto& steer = registry.GetComponent<ECS::SteeringComponent>(selectedEntity);

          ImGui::Separator();
          if (ImGui::CollapsingHeader("Steering", ImGuiTreeNodeFlags_DefaultOpen)) {

            ImGui::Checkbox("Seek", &steer.seekEnabled);
            ImGui::SameLine();
            ImGui::Checkbox("Flee", &steer.fleeEnabled);
            ImGui::SameLine();
            ImGui::Checkbox("Arrive", &steer.arriveEnabled);

            ImGui::DragFloat("Max Speed", &steer.maxSpeed, 1.f, 0.f, 2000.f);
            ImGui::DragFloat("Max Force", &steer.maxForce, 1.f, 0.f, 2000.f);
            ImGui::DragFloat("Slowing Radius", &steer.slowingRadius, 1.f, 0.f, 2000.f);

            // Target: se edita directamente el EntityID (uint64_t).
            // Mismo criterio de solo-lectura/edición simple que usa
            // Camera::followTarget, pero aquí sí es editable.
            ImGuiDataType_ entityIdType = ImGuiDataType_U64;
            ImGui::InputScalar("Target Entity", entityIdType, &steer.target);

            if (steer.target == ECS::NULL_ENTITY)
              ImGui::TextDisabled("Target: (ninguno)");
            else if (!registry.IsAlive(steer.target))
              ImGui::TextColored(ImVec4(1.f, 0.4f, 0.4f, 1.f), "Target: entidad no valida");
            else
              ImGui::Text("Target: %llu", static_cast<unsigned long long>(steer.target));
          }
        }
      }
      ImGui::End();
		}


	private:
		ECS::EntityID selectedEntity = ECS::NULL_ENTITY;
    bool m_initialized = false;
	};

};

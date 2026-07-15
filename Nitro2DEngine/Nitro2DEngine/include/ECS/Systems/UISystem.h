#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/SteeringComponent.h"
#include "ECS/Components/Obstacle.h"
#include "ECS/Editor/ComponentRegistry.h"
#include "ECS/Components/Name.h"

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


    /**
			* @brief Se llama cada frame, aquí va la lógica principal del sistema.
      * 
			* @param registry Referencia al Registry del sistema.
			* @param deltaTime Tiempo transcurrido desde el último frame.
      * 
			* @note Este método maneja la interfaz de usuario, incluyendo el Outliner y el Inspector,
      */
		void 
		OnUpdate(Registry& registry, float deltaTime) override {
      if (!m_initialized)
      {
        ApplyStyle();
        m_initialized = true;
      }

      if (m_nameWarningTimer > 0.f) {
        m_nameWarningTimer -= deltaTime;
      }

      Outliner(registry);
      Inspector(registry);
		}
    
		/**
			* @brief Muestra la ventana de Outliner, que lista todas las entidades en el registro.
      * 
			* @param registry Referencia al Registry del sistema.
      * 
			* @note Este método crea una ventana de ImGui que lista todas las entidades vivas en el registro.
      */
		void 
		Outliner(Registry& registry) {
			ImGui::Begin("Hierarchy");

			for (auto entity : registry.GetAllEntities()) {
				if (!registry.IsAlive(entity)) continue;

        std::string label = EntityLabel(registry, entity);

				if (ImGui::Selectable(label.c_str(), selectedEntity == entity))
				{
					selectedEntity = entity;
				}
			}

      ImGui::End();
		}

		/**
			* @brief Muestra la ventana de Inspector, que permite ver y modificar los componentes de la entidad seleccionada.
      * 
			* @param registry Referencia al Registry del sistema.
      * 
			* @note Este método crea una ventana de ImGui que muestra los componentes de la entidad 
      * seleccionada y permite agregar, eliminar o modificar sus propiedades.
      */
    void
    Inspector(Registry& registry) {
      ImGui::Begin("Inspector");

      if (!registry.IsAlive(selectedEntity)) {
        ImGui::Text("No hay entidad seleccionada");
        ImGui::End();
        return;
      }

      if (ImGui::Button("+ Add Component")) {
        ImGui::OpenPopup("AddComponentPopup");
      }

      if (ImGui::BeginPopup("AddComponentPopup")) {
        for (auto& info : ECS::Editor::ComponentRegistry::Instance().GetTypes()) {
          if (info.hasComponent(registry, selectedEntity)) continue; // ya lo tiene

          if (ImGui::Selectable(info.name.c_str())) {
            info.addComponent(registry, selectedEntity);
          }
        }
        ImGui::EndPopup();
      }

      ImGui::Separator();

      // --- Name --------------------------------------------------
      if (registry.HasComponent<ECS::Name>(selectedEntity))
      {
        bool nameOpen = ImGui::CollapsingHeader("Name",
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);

        if (RemoveComponentButton("X##Name")) {
          registry.RemoveComponent<ECS::Name>(selectedEntity);
          ImGui::End();
          return;
        }

        if (nameOpen) {
          auto& nameComp = registry.GetComponent<ECS::Name>(selectedEntity);

          char buffer[128];
          std::snprintf(buffer, sizeof(buffer), "%s", nameComp.name.c_str());

          if (ImGui::InputText("##NameInput", buffer, sizeof(buffer))) {
            std::string newName(buffer);

            if (newName.empty()) {
              m_nameWarningTimer = 2.f;
            }
            else {
              nameComp.name = std::move(newName);
            }
          }

          if (m_nameWarningTimer > 0.f) {
            ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), "El nombre no puede quedar vacio");
          }
        }
      }

      // --- Transform (no se elimina: el resto del Inspector lo asume) ---
      {
        bool transformOpen = ImGui::CollapsingHeader("Transform",
          ImGuiTreeNodeFlags_DefaultOpen);

        if (transformOpen) {
          auto& transform = registry.GetComponent<ECS::Transform>(selectedEntity);

          ImGui::DragFloat2("Position", (float*)&transform.position, 1.0f);
          ImGui::DragFloat("Rotation", &transform.rotation, 1.0f);
          ImGui::DragFloat2("Scale", (float*)&transform.scale, 0.01f);
        }
      }

      // --- Render --------------------------------------------------
      if (registry.HasComponent<ECS::Render>(selectedEntity))
      {
        bool renderOpen = ImGui::CollapsingHeader("Render",
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);

        if (RemoveComponentButton("X##Render")) {
          registry.RemoveComponent<ECS::Render>(selectedEntity);
          ImGui::End();
          return;
        }

        if (renderOpen) {
          auto& render = registry.GetComponent<ECS::Render>(selectedEntity);

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
      }

      // --- Camera --------------------------------------------------
      // Antes vivía anidado dentro de Render: si la entidad no tenía Render,
      // Camera era invisible e inmodificable aunque existiera. Corregido:
      // ahora es independiente, igual que Steering y Obstacle.
      if (auto* cam = registry.TryGetComponent<ECS::Camera>(selectedEntity)) {
        bool camOpen = ImGui::CollapsingHeader("Camera",
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);

        if (RemoveComponentButton("X##Camera")) {
          registry.RemoveComponent<ECS::Camera>(selectedEntity);
          ImGui::End();
          return;
        }

        if (camOpen) {
          ImGui::Checkbox("Active", &cam->active);
          ImGui::DragFloat("Zoom", &cam->zoom, 0.01f, 0.05f, 10.f);
          ImGui::DragFloat("Follow Speed", &cam->followSpeed, 0.1f, 0.f, 50.f);

          if (cam->followTarget == ECS::NULL_ENTITY)
            ImGui::Text("Follow Target: (ninguno)");
          else
            ImGui::Text("Follow Target: %llu",
              static_cast<unsigned long long>(cam->followTarget));
        }
      }

      // --- Steering --------------------------------------------------
      if (registry.HasComponent<ECS::SteeringComponent>(selectedEntity))
      {
        bool steeringOpen = ImGui::CollapsingHeader("Steering",
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);

        if (RemoveComponentButton("X##Steering")) {
          registry.RemoveComponent<ECS::SteeringComponent>(selectedEntity);
          ImGui::End();
          return;
        }

        if (steeringOpen) {
          auto& steer = registry.GetComponent<ECS::SteeringComponent>(selectedEntity);

          ImGui::Checkbox("Seek", &steer.seekEnabled);
          ImGui::SameLine();
          ImGui::Checkbox("Flee", &steer.fleeEnabled);
          ImGui::SameLine();
          ImGui::Checkbox("Arrive", &steer.arriveEnabled);

          ImGui::Checkbox("Pursuit", &steer.pursuitEnabled);
          ImGui::SameLine();
          ImGui::Checkbox("Wander", &steer.wanderEnabled);
          ImGui::SameLine();
          ImGui::Checkbox("Avoid Obstacles", &steer.obstacleAvoidanceEnabled);

          ImGui::DragFloat("Max Speed", &steer.maxSpeed, 1.f, 0.f, 2000.f);
          ImGui::DragFloat("Max Force", &steer.maxForce, 1.f, 0.f, 2000.f);
          ImGui::DragFloat("Slowing Radius", &steer.slowingRadius, 1.f, 0.f, 2000.f);

          {
            std::string previewLabel = (steer.target == ECS::NULL_ENTITY)
              ? "(ninguno)"
              : EntityLabel(registry, steer.target);

            if (ImGui::BeginCombo("Target Entity", previewLabel.c_str())) {

              bool noneSelected = (steer.target == ECS::NULL_ENTITY);
              if (ImGui::Selectable("(ninguno)", noneSelected)) {
                steer.target = ECS::NULL_ENTITY;
              }

              for (auto candidate : registry.GetAllEntities()) {
                if (!registry.IsAlive(candidate)) continue;
                if (candidate == selectedEntity) continue;

                std::string label = EntityLabel(registry, candidate);
                bool isSelected = (steer.target == candidate);
                if (ImGui::Selectable(label.c_str(), isSelected)) {
                  steer.target = candidate;
                }
              }
              ImGui::EndCombo();
            }
          }

          if (steer.pursuitEnabled) {
            ImGui::DragFloat("Prediction Time", &steer.pursuitPredictionTime, 0.05f, 0.f, 5.f);
          }

          if (steer.wanderEnabled) {
            ImGui::DragFloat("Wander Radius", &steer.wanderRadius, 0.5f, 0.f, 500.f);
            ImGui::DragFloat("Wander Distance", &steer.wanderDistance, 0.5f, 0.f, 500.f);
            ImGui::DragFloat("Wander Jitter", &steer.wanderJitter, 0.05f, 0.f, 20.f);
          }

          if (steer.obstacleAvoidanceEnabled) {
            ImGui::DragFloat("Look Ahead", &steer.obstacleLookAhead, 1.f, 0.f, 1000.f);
            ImGui::DragFloat("Agent Radius", &steer.obstacleRadius, 0.5f, 0.f, 200.f);
          }
        }
      }

      // --- Obstacle --------------------------------------------------
      if (registry.HasComponent<ECS::Obstacle>(selectedEntity))
      {
        bool obstacleOpen = ImGui::CollapsingHeader("Obstacle",
          ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowOverlap);

        if (RemoveComponentButton("X##Obstacle")) {
          registry.RemoveComponent<ECS::Obstacle>(selectedEntity);
          ImGui::End();
          return;
        }

        if (obstacleOpen) {
          auto& obstacle = registry.GetComponent<ECS::Obstacle>(selectedEntity);
          ImGui::DragFloat("Radius", &obstacle.radius, 0.5f, 1.f, 500.f);
        }
      }

      ImGui::End();
    }


	private:
		ECS::EntityID selectedEntity = ECS::NULL_ENTITY;///< ID de la entidad actualmente seleccionada en el Outliner.
		bool m_initialized = false;///< Indica si el sistema ha sido inicializado (para aplicar el estilo de ImGui una sola vez).
		float m_nameWarningTimer = 0.f;///< Temporizador para mostrar advertencia de nombre vacío en el Inspector.

    /**
			* @brief Devuelve la etiqueta de una entidad para mostrar en la interfaz de usuario.
      * 
			* @param registry Referencia al Registry del sistema.
			* @param entity La ID de la entidad de la que se quiere obtener la etiqueta.
      * 
			* @return std::string La etiqueta de la entidad, que es su nombre si tiene un componente Name, o "Entity <ID>" si no lo tiene.
      * 
			* @note Este método se utiliza para mostrar una representación legible de la entidad 
      * en el Outliner y en otros lugares de la interfaz de usuario.
      */
    [[nodiscard]] std::string
      EntityLabel(Registry& registry, EntityID entity) const {
      if (auto* n = registry.TryGetComponent<ECS::Name>(entity)) {
        return n->name;
      }
      return "Entity " + std::to_string(entity);
    }

		/**
			* @brief Muestra un botón de eliminación de componente en el Inspector.
      * 
			* @param label La etiqueta del botón.
      * 
			* @return true si el botón fue presionado, false en caso contrario.
      * 
      * @note Este método se utiliza para mostrar un botón de eliminación de componente alineado a la derecha en el Inspector.
			*/
    bool
    RemoveComponentButton(const char* label) {
      ImGui::SameLine(ImGui::GetWindowWidth() - 60.f);
      return ImGui::SmallButton(label);
    }
	};

};

#pragma once
#include "Prerequisites.h"
#include "ECS/System.h"
#include "ECS/Registry.h"
#include "ECS/EntityFactory.h"
#include "ECS/Editor/ComponentRegistry.h"
#include "ECS/Components/Transform.h"
#include "ECS/Components/Render.h"
#include "ECS/Components/Camera.h"
#include "ECS/Components/SteeringComponent.h"
#include "ECS/Components/SteeringDebugComponent.h"
#include "ECS/Components/Obstacle.h"
#include "ECS/Components/Name.h"
#include "ECS/Components/PathComponent.h"
#include "ECS/Components/PathEditorComponent.h"
#include "ECS/Components/DebugPathComponent.h"
#include "ECS/PathUtils.h"
#include "ECS/Components/StartingGridComponent.h"
#include "ECS/Components/RaceParticipantComponent.h"
#include "ECS/StartingGridUtils.h"
#include "ECS/Components/RaceManagerComponent.h"
#include "ECS/Components/MovementControlComponent.h"
#include "ECS/RaceUtils.h"

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

      if (m_textureMessageTimer > 0.f)
      {
        m_textureMessageTimer -= deltaTime;

        if (m_textureMessageTimer <= 0.f)
        {
          m_textureLoadSucceeded = false;
          m_textureLoadFailed = false;
        }
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

      // --------------------------------------------------
      // Entity creation controls
      // --------------------------------------------------

      if (ImGui::Button("+ Create Empty",ImVec2(-1.f, 0.f)))
      {
        const std::string entityName = GenerateUniqueEntityName(registry);

        const EntityID newEntity = ECS::CreateEntity(registry, entityName, { 0.f, 0.f });

        // Seleccionar inmediatamente la nueva entidad.
        selectedEntity = newEntity;
      }

      ImGui::Separator();

      // --------------------------------------------------
      // Entity list
      // --------------------------------------------------

      for (const EntityID entity : registry.GetAllEntities())
      {
        if (!registry.IsAlive(entity)) continue;

        const std::string label = EntityLabel(registry, entity);

        // El sufijo oculto garantiza un ID único para ImGui,
        // incluso si dos entidades terminan teniendo el mismo nombre.
        const std::string imguiLabel = label + "##Entity_" + std::to_string(entity);

        if (ImGui::Selectable(imguiLabel.c_str(), selectedEntity == entity))
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

      ImGui::SameLine();

      if (ImGui::Button("- Remove Component"))
      {
        ImGui::OpenPopup("RemoveComponentPopup");
      }


      if (ImGui::BeginPopup("RemoveComponentPopup"))
      {
        auto& componentRegistry = ECS::Editor::ComponentRegistry::Instance();

        bool foundComponent = false;

        for (const auto& info : componentRegistry.GetTypes())
        {
          // Solo mostrar componentes que realmente
          // estén presentes en la entidad.
          if (!info.hasComponent(registry, selectedEntity)) continue;

          foundComponent = true;

          std::string blockedBy;

          const bool canRemove =componentRegistry.CanRemove(registry,
                                                            selectedEntity,
                                                            info,
                                                            &blockedBy);

          ImGui::BeginDisabled(!canRemove);

          if (ImGui::Selectable(info.name.c_str()))
          {
            info.removeComponent(registry, selectedEntity);

            ImGui::CloseCurrentPopup();
          }

          ImGui::EndDisabled();


          // Explicar por qué está bloqueado.
          if (!canRemove && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
          {
            ImGui::SetTooltip("Required by: %s", blockedBy.c_str());
          }
        }


        if (!foundComponent)
        {
          ImGui::TextDisabled("No removable components.");
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
        const bool renderOpen =ImGui::CollapsingHeader(
            "Render",
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_AllowOverlap);

        if (RemoveComponentButton("X##Render"))
        {
          registry.RemoveComponent<ECS::Render>(selectedEntity);

          // Invalidar el buffer, porque ya no existe
          // un Render asociado a esta selección.
          m_textureBufferEntity = ECS::NULL_ENTITY;

          m_texturePathBuffer.fill('\0');

          ImGui::End();
          return;
        }

        if (renderOpen)
        {
          auto& render = registry.GetComponent<ECS::Render>(selectedEntity);
          DrawRenderInspector(selectedEntity, render);
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

			// --- Path --------------------------------------------------
      if (auto* path = registry.TryGetComponent<ECS::PathComponent>(selectedEntity))
      {
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Path",ImGuiTreeNodeFlags_DefaultOpen))
        {
          ImGui::Text("Control Points: %llu",static_cast<unsigned long long>(
              path->controlPoints.size()));

          ImGui::Text("Sampled Points: %llu",static_cast<unsigned long long>(
              path->points.size()));

          if (path->controlPoints.empty())
          {
            ImGui::TextDisabled("Path is empty.");
          }
          else if (path->controlPoints.size() < 4)
          {
            ImGui::TextColored(
              ImVec4(1.f, 0.75f, 0.2f, 1.f), "Add at least 4 control points for Catmull-Rom.");
          }
          else
          {
            ImGui::TextColored(
              ImVec4(0.3f, 1.f, 0.5f, 1.f), "Catmull-Rom path generated.");
          }

          ImGui::Checkbox("Closed Path", &path->closed);

          ImGui::DragFloat("Path Radius",&path->radius, 1.f, 0.f, 1000.f);

          int samples =path->samplesPerSegment;

          if (ImGui::DragInt("Samples Per Segment", &samples, 1.f, 1, 100))
          {
            path->samplesPerSegment = std::max(1, samples);

            ECS::RebuildPath(*path);
          }
        }
      }

			// --- Debug Path --------------------------------------------------
      if (auto* debug = registry.TryGetComponent<ECS::DebugPathComponent>(selectedEntity))
      {
        ImGui::Separator();

        if (ImGui::CollapsingHeader(
          "Path Debug",
          ImGuiTreeNodeFlags_DefaultOpen))
        {
          ImGui::Checkbox(
            "Debug Enabled",
            &debug->enabled);

          ImGui::BeginDisabled(!debug->enabled);

          ImGui::Checkbox(
            "Draw Center Line",
            &debug->drawCenterLine);

          ImGui::Checkbox(
            "Draw Path Radius",
            &debug->drawPathRadius);

          ImGui::Checkbox(
            "Draw Sample Points",
            &debug->drawSamplePoints);

          if (debug->drawSamplePoints)
          {
            ImGui::DragInt(
              "Sample Point Step",
              &debug->samplePointStep,
              1.f,
              1,
              100);
          }

          ImGui::EndDisabled();
        }
      }

      // --- Path Editor -------------------------------------------
      if (auto* pathEditor =registry.TryGetComponent<ECS::PathEditorComponent>(selectedEntity))
      {
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Path Editor", ImGuiTreeNodeFlags_DefaultOpen))
        {
          // ==================================================
          // Enable / Disable editor
          // ==================================================

          bool editEnabled = pathEditor->enabled;

          if (ImGui::Checkbox("Edit Path", &editEnabled))
          {
            // Si vamos a activar este editor,
            // desactivar cualquier otro Path Editor.
            if (editEnabled)
            {
              registry
                .GetView<ECS::PathEditorComponent>()
                .Each(
                  [this](
                    ECS::EntityID entity,
                    ECS::PathEditorComponent& otherEditor)
                  {
                    if (entity == selectedEntity) return;

                    otherEditor.enabled = false;

                    otherEditor.mode = ECS::PathEditMode::None;

                    otherEditor.selectedControlPoint = -1;

                    otherEditor.draggingControlPoint = false;
                  });
            }

            pathEditor->enabled = editEnabled;

            // Al apagar la edición limpiamos
            // cualquier estado temporal.
            if (!pathEditor->enabled)
            {
              pathEditor->mode = ECS::PathEditMode::None;

              pathEditor->selectedControlPoint = -1;

              pathEditor->draggingControlPoint = false;
            }
          }


          ImGui::BeginDisabled(!pathEditor->enabled);


          // ==================================================
          // Edit Mode
          // ==================================================

          const char* currentMode = PathEditModeLabel(pathEditor->mode);

          if (ImGui::BeginCombo("Edit Mode", currentMode))
          {
            const PathEditMode modes[] = {
                PathEditMode::None,
                PathEditMode::Add,
                PathEditMode::Move,
                PathEditMode::Delete
            };

            for (const PathEditMode mode : modes)
            {
              const bool selected = pathEditor->mode == mode;

              if (ImGui::Selectable(PathEditModeLabel(mode), selected))
              {
                pathEditor->mode = mode;

                // Cambiar de herramienta cancela
                // selección y drag anteriores.
                pathEditor->selectedControlPoint = -1;

                pathEditor->draggingControlPoint = false;
              }

              if (selected)
              {
                ImGui::SetItemDefaultFocus();
              }
            }

            ImGui::EndCombo();
          }


          // ==================================================
          // Instructions
          // ==================================================

          switch (pathEditor->mode)
          {
          case ECS::PathEditMode::None:
            ImGui::TextDisabled(
              "Select an editing tool.");
            break;

          case ECS::PathEditMode::Add:
            ImGui::TextColored(
              ImVec4(
                0.3f,
                1.f,
                0.6f,
                1.f),
              "Left click on the scene to add points.");
            break;

          case ECS::PathEditMode::Move:
            ImGui::TextColored(
              ImVec4(
                0.3f,
                1.f,
                0.6f,
                1.f),
              "Click and drag a control point to move it.");
            break;

          case ECS::PathEditMode::Delete:
            ImGui::TextColored(
              ImVec4(
                1.f,
                0.5f,
                0.3f,
                1.f),
              "Click a control point to delete it.");
            break;
          }


          // ==================================================
          // Selection Settings
          // ==================================================

          ImGui::Separator();

          ImGui::DragFloat("Pick Radius", &pathEditor->pointPickRadius, 0.5f, 1.f, 200.f);

          ImGui::Checkbox("Show Control Points", &pathEditor->showControlPoints);

          if (pathEditor->showControlPoints)
          {
            ImGui::DragFloat("Control Point Size", &pathEditor->controlPointDrawRadius, 0.25f, 1.f, 50.f);
          }


          // ==================================================
          // Current Selection
          // ==================================================

          ImGui::Separator();

          if (pathEditor->selectedControlPoint >= 0)
          {
            ImGui::Text("Selected Point: %d", pathEditor->selectedControlPoint);

            if (ImGui::Button("Clear Selection"))
            {
              pathEditor->selectedControlPoint = -1;

              pathEditor->draggingControlPoint = false;
            }
          }
          else
          {
            ImGui::TextDisabled("Selected Point: none");
          }


          // ==================================================
          // Path operations
          // ==================================================

          if (auto* path =registry.TryGetComponent<ECS::PathComponent>(selectedEntity))
          {
            ImGui::Separator();
            ImGui::TextUnformatted("Path Operations");

            if (ImGui::Button("Rebuild Path"))
            {
              ECS::RebuildPath(*path);
            }


            ImGui::BeginDisabled(path->controlPoints.empty());

            if (ImGui::Button("Clear All Points"))
            {
              ImGui::OpenPopup("ConfirmClearPathPoints");
            }

            ImGui::EndDisabled();


            // ==============================================
            // Confirmation popup
            // ==============================================

            if (ImGui::BeginPopupModal("ConfirmClearPathPoints", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
              ImGui::TextUnformatted("Delete all control points?");

              ImGui::TextDisabled("This action cannot be undone.");

              ImGui::Separator();

              if (ImGui::Button("Delete All", ImVec2(120.f, 0.f)))
              {
                path->controlPoints.clear();

                ECS::RebuildPath(*path);

                pathEditor->selectedControlPoint = -1;

                pathEditor->draggingControlPoint = false;

                ImGui::CloseCurrentPopup();
              }

              ImGui::SameLine();

              if (ImGui::Button("Cancel", ImVec2(120.f, 0.f)))
              {
                ImGui::CloseCurrentPopup();
              }

              ImGui::EndPopup();
            }
          }


          ImGui::EndDisabled();
        }
      }

      // --- Race Manager -----------------------------------------
      if (auto* race = registry.TryGetComponent<ECS::RaceManagerComponent>(selectedEntity))
      {
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Race Manager", ImGuiTreeNodeFlags_DefaultOpen))
        {
          // ==============================================
          // Current State
          // ==============================================

          const char* stateLabel = "Unknown";

          switch (race->state)
          {
          case ECS::RaceState::Waiting:
            stateLabel = "Waiting";
            break;
             
          case ECS::RaceState::Racing:
            stateLabel = "Racing";
            break;

          case ECS::RaceState::Finished:
            stateLabel = "Finished";
            break;
          }

          ImGui::Text("State: %s", stateLabel);


          // ==============================================
          // Starting Grid selector
          // ==============================================

          std::string gridPreview =
            race->startingGridEntity ==
            ECS::NULL_ENTITY
            ? "(none)"
            : EntityLabel(
              registry,
              race->startingGridEntity);

          if (ImGui::BeginCombo("Starting Grid", gridPreview.c_str()))
          {
            if (ImGui::Selectable("(none)", race->startingGridEntity == ECS::NULL_ENTITY))
            {
              race->startingGridEntity = ECS::NULL_ENTITY;
            }

            for (const EntityID candidate : registry.GetAllEntities())
            {
              if (!registry.IsAlive(candidate))
              {
                continue;
              }

              if (!registry.HasComponent<ECS::StartingGridComponent>(candidate))
              {
                continue;
              }

              const std::string label =EntityLabel(registry, candidate);

              const bool selected = candidate == race->startingGridEntity;

              if (ImGui::Selectable(label.c_str(), selected))
              {
                race->startingGridEntity = candidate;
              }
            }

            ImGui::EndCombo();
          }

          // ==============================================
          // Randomization
          // ==============================================

          ImGui::Separator();
          ImGui::TextUnformatted("Kart Randomization");

          ImGui::DragFloat("Min Speed", &race->minRandomSpeed, 1.f, 0.f, 1000.f);

          ImGui::DragFloat("Max Speed", &race->maxRandomSpeed, 1.f, 0.f, 1000.f);

					// Permitimos randomizar solo si la carrera no ha terminado, para evitar inconsistencias.
          const bool canRandomize = race->state != ECS::RaceState::Finished;

          ImGui::BeginDisabled(!canRandomize);

          if (ImGui::Button("RANDOMIZE KARTS", ImVec2(-1.f, 32.f)))
          {
            ECS::RandomizeRaceParticipants(registry,selectedEntity);
          }

          ImGui::EndDisabled();

          ImGui::Separator();
          ImGui::TextUnformatted("Participants");

          int participantCount = 0;

          registry
            .GetView<
            ECS::RaceParticipantComponent,
            ECS::SteeringComponent>()
            .Each(
              [&](ECS::EntityID entity,
                ECS::RaceParticipantComponent& participant,
                ECS::SteeringComponent& steering)
              {
                if (participant.raceManagerEntity !=
                  selectedEntity)
                {
                  return;
                }

                ++participantCount;

                const std::string label =
                  EntityLabel(
                    registry,
                    entity);

                ImGui::Text(
                  "%s  -  Max Speed: %.1f",
                  label.c_str(),
                  steering.maxSpeed);
              });

          if (participantCount == 0)
          {
            ImGui::TextDisabled(
              "No participants assigned.");
          }

          // ==============================================
          // Start Race
          // ==============================================

          ImGui::Separator();

          if (race->state == ECS::RaceState::Waiting)
          {
            const bool canStart = race->startingGridEntity != ECS::NULL_ENTITY;

            ImGui::BeginDisabled(!canStart);

            if (ImGui::Button("START RACE", ImVec2(-1.f, 40.f)))
            {
              race->state = ECS::RaceState::Racing;
            }

            ImGui::EndDisabled();

            if (!canStart)
            {
              ImGui::TextDisabled("Select a Starting Grid before starting.");
            }
          }
          else if (race->state == ECS::RaceState::Racing)
          {
            ImGui::TextColored(ImVec4( 0.2f, 1.f, 0.4f, 1.f), "Race in progress");
          }
          else
          {
            ImGui::Text("Race finished");
          }
        }
      }

      // --- Starting Grid -----------------------------------------
      if (auto* grid = registry.TryGetComponent<ECS::StartingGridComponent>(selectedEntity))
      {
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Starting Grid", ImGuiTreeNodeFlags_DefaultOpen))
        {
          bool rebuildGrid = false;

          // ==============================================
          // Path Entity
          // ==============================================

          std::string pathPreview = grid->pathEntity == ECS::NULL_ENTITY? "(none)": EntityLabel(registry, grid->pathEntity);

          if (ImGui::BeginCombo("Path Entity", pathPreview.c_str()))
          {
            if (ImGui::Selectable("(none)", grid->pathEntity == ECS::NULL_ENTITY))
            {
              grid->pathEntity = ECS::NULL_ENTITY;

              grid->slots.clear();
            }

            for (const EntityID candidate : registry.GetAllEntities())
            {
              if (!registry.IsAlive(candidate))
              {
                continue;
              }

              if (!registry.HasComponent<ECS::PathComponent>(candidate))
              {
                continue;
              }

              const std::string label =EntityLabel(registry, candidate);

              const bool selected = candidate == grid->pathEntity;

              if (ImGui::Selectable(label.c_str(), selected))
              {
                grid->pathEntity = candidate;

                rebuildGrid = true;
              }
            }

            ImGui::EndCombo();
          }


          // ==============================================
          // Grid parameters
          // ==============================================

          rebuildGrid |= ImGui::DragInt("Finish Point Index", &grid->anchorPointIndex, 1.f, 0, 100000);

          rebuildGrid |= ImGui::DragFloat("Finish Line Half Width", &grid->lineHalfWidth, 1.f, 5.f, 1000.f);

          rebuildGrid |= ImGui::DragInt("Slot Count", &grid->slotCount, 1.f, 1, 20);

          rebuildGrid |= ImGui::DragFloat("First Row Offset", &grid->firstRowOffset, 1.f, 0.f, 1000.f);

          rebuildGrid |= ImGui::DragFloat("Row Spacing", &grid->rowSpacing, 1.f, 1.f, 500.f);

          rebuildGrid |= ImGui::DragFloat("Column Spacing", &grid->columnSpacing, 1.f, 1.f, 500.f);

          ImGui::Checkbox("Visible##StartingGrid", &grid->visible);

          EditSFMLColor("Finish Line Color", grid->finishLineColor);

          EditSFMLColor("Slot Color", grid->slotColor);


          if (rebuildGrid)
          {
            ECS::RebuildStartingGrid(registry, *grid);
          }

          if (grid->pathEntity == ECS::NULL_ENTITY)
          {
            ImGui::TextDisabled("Select a Path Entity.");
          }
          else
          {
            ImGui::Text("Generated Slots: %llu", static_cast<unsigned long long>(grid->slots.size()));
          }

          if (ImGui::Button("Rebuild Starting Grid"))
          {
            ECS::RebuildStartingGrid(registry, *grid);
          }
        }
      }

      // --- Race Participant --------------------------------------
      if (auto* participant = registry.TryGetComponent<ECS::RaceParticipantComponent>(selectedEntity))
      {
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Race Participant", ImGuiTreeNodeFlags_DefaultOpen))
        {
          // ==============================================
          // Race Manager
          // ==============================================

          std::string racePreview =
            participant->raceManagerEntity ==
            ECS::NULL_ENTITY
            ? "(none)"
            : EntityLabel(
              registry,
              participant->raceManagerEntity);

          if (ImGui::BeginCombo("Race Manager", racePreview.c_str()))
          {
            if (ImGui::Selectable("(none)", participant->raceManagerEntity == ECS::NULL_ENTITY))
            {
              participant->raceManagerEntity = ECS::NULL_ENTITY;
            }

            for (const EntityID candidate : registry.GetAllEntities())
            {
              if (!registry.IsAlive(candidate))
              {
                continue;
              }

              if (!registry.HasComponent<ECS::RaceManagerComponent>(candidate))
              {
                continue;
              }

              const std::string label = EntityLabel(registry, candidate);

              const bool selected = participant->raceManagerEntity == candidate;

              if (ImGui::Selectable(label.c_str(), selected))
              {
                participant->raceManagerEntity = candidate;
              }
            }

            ImGui::EndCombo();
          }

          // ==============================================
          // Starting Grid selector
          // ==============================================

          std::string gridPreview =
            participant->startingGridEntity ==
            ECS::NULL_ENTITY
            ? "(none)"
            : EntityLabel(
              registry,
              participant->startingGridEntity);

          if (ImGui::BeginCombo("Starting Grid", gridPreview.c_str()))
          {
            if (ImGui::Selectable("(none)", participant->startingGridEntity == ECS::NULL_ENTITY))
            {
              participant->startingGridEntity = ECS::NULL_ENTITY;

              participant->startingSlot = -1;
            }

            for (const EntityID candidate : registry.GetAllEntities())
            {
              if (!registry.IsAlive(candidate))
              {
                continue;
              }

              if (!registry.HasComponent<ECS::StartingGridComponent>(candidate))
              {
                continue;
              }

              const std::string label =EntityLabel(registry, candidate);

              const bool selected = participant->startingGridEntity == candidate;

              if (ImGui::Selectable(label.c_str(), selected))
              {
                participant->startingGridEntity = candidate;

                participant->startingSlot = -1;
              }
            }

            ImGui::EndCombo();
          }


          // ==============================================
          // Starting Position
          // ==============================================

          if (participant->startingGridEntity !=
            ECS::NULL_ENTITY &&
            registry.IsAlive(
              participant->
              startingGridEntity))
          {
            auto* grid = registry.TryGetComponent<ECS::StartingGridComponent>(participant->startingGridEntity);

            if (grid && !grid->slots.empty())
            {
              const std::string slotPreview =
                participant->startingSlot >= 0
                ? "Slot " +
                std::to_string(
                  participant->
                  startingSlot + 1)
                : "(none)";

              if (ImGui::BeginCombo("Starting Position", slotPreview.c_str()))
              {
                if (ImGui::Selectable("(none)", participant->startingSlot < 0))
                {
                  participant->startingSlot = -1;
                }

                for (std::size_t i = 0; i < grid->slots.size(); ++i)
                {
                  const std::string label = "Slot " + std::to_string(i + 1);

                  const bool selected = participant->startingSlot == static_cast<int>(i);

                  if (ImGui::Selectable(label.c_str(), selected))
                  {
                    participant->startingSlot = static_cast<int>(i);

                    // ==========================
                    // SNAP AUTOMÁTICO
                    // ==========================

                    auto* transform = registry.TryGetComponent<ECS::Transform>(selectedEntity);

                    if (transform)
                    {
                      const StartingGridSlot& slot =grid->slots[i];

                      transform->position = slot.position;

                      transform->rotation = slot.rotation;
                    }
                  }
                }

                ImGui::EndCombo();
              }
            }
            else
            {
              ImGui::TextDisabled("Starting Grid has no slots.");
            }
          }
          else
          {
            ImGui::TextDisabled("Select a Starting Grid.");
          }
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

          ImGui::Checkbox("Path Following", &steer.pathFollowingEnabled);
          ImGui::SameLine();         
          ImGui::Checkbox("Separation", &steer.separationEnabled);

          ImGui::DragFloat("Max Speed", &steer.maxSpeed, 1.f, 0.f, 2000.f);
          ImGui::DragFloat("Max Force", &steer.maxForce, 1.f, 0.f, 2000.f);
          ImGui::DragFloat("Slowing Radius", &steer.slowingRadius, 1.f, 0.f, 2000.f);

          // --------------------------------------------------
          // Path Following
          // --------------------------------------------------

          if (steer.pathFollowingEnabled)
          {
            ImGui::Separator();
            ImGui::TextUnformatted("Path Following");

            std::string pathPreview =
              steer.pathEntity == ECS::NULL_ENTITY
              ? "(ninguno)"
              : EntityLabel(
                registry,
                steer.pathEntity);

            if (ImGui::BeginCombo("Path Entity", pathPreview.c_str()))
            {
              const bool noneSelected =steer.pathEntity == ECS::NULL_ENTITY;

              if (ImGui::Selectable("(ninguno)", noneSelected))
              {
                steer.pathEntity = ECS::NULL_ENTITY;
              }

              for (auto candidate : registry.GetAllEntities())
              {
                if (!registry.IsAlive(candidate))continue;

                // Solo mostrar entidades que realmente
                // tengan un PathComponent.
                if (!registry.HasComponent<ECS::PathComponent>(candidate)) continue;

                const std::string label = EntityLabel(registry, candidate);

                const bool isSelected = steer.pathEntity == candidate;

                if (ImGui::Selectable(label.c_str(), isSelected))
                {
                  steer.pathEntity = candidate;
                }
              }

              ImGui::EndCombo();
            }

            ImGui::DragFloat("Path Ahead Distance", &steer.pathAheadDistance, 1.f, 0.f, 1000.f);
          }

          // --------------------------------------------------
          // Separation
          // --------------------------------------------------

          if (steer.separationEnabled)
          {
            ImGui::Separator();
            ImGui::TextUnformatted("Separation");

            ImGui::DragFloat("Separation Radius", &steer.separationRadius, 1.f, 0.f, 500.f);

            ImGui::DragFloat("Separation Strength", &steer.separationStrength, 0.05f, 0.f, 10.f);
          }

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

      if (auto* debug =
        registry.TryGetComponent<ECS::SteeringDebugComponent>(
          selectedEntity))
      {
        ImGui::Separator();

        if (ImGui::CollapsingHeader("Steering Debug", ImGuiTreeNodeFlags_DefaultOpen))
        {
          ImGui::Checkbox("Debug Enabled##SteeringDebug", &debug->enabled);

          ImGui::BeginDisabled(!debug->enabled);

          // ----------------------------------------------
          // Escalas
          // ----------------------------------------------

          ImGui::DragFloat("Velocity Scale", &debug->velocityScale, 0.01f, 0.01f, 5.f);

          ImGui::DragFloat("Force Scale", &debug->forceScale, 0.01f, 0.01f, 5.f);

          ImGui::SeparatorText("Velocity");

          ImGui::Checkbox("Draw Velocity", &debug->drawVelocity);

          EditSFMLColor("Velocity Color", debug->velocityColor);

          ImGui::SeparatorText("Path Geometry");

          ImGui::Checkbox("Draw Predicted Position", &debug->drawPredictedPosition);

          EditSFMLColor("Predicted Position Color", debug->predictedPositionColor);

          ImGui::Checkbox("Draw Nearest Path Point", &debug->drawNearestPathPoint);

          EditSFMLColor("Nearest Path Point Color", debug->nearestPathPointColor);

          ImGui::Checkbox("Draw Path Target Point", &debug->drawPathTargetPoint);

          EditSFMLColor("Path Target Point Color", debug->pathTargetPointColor);

          ImGui::SeparatorText("Steering Forces");

          ImGui::Checkbox("Draw Path Following Force", &debug->drawPathFollowingForce);

          EditSFMLColor("Path Following Force Color", debug->pathFollowingForceColor);

          ImGui::Checkbox("Draw Separation Force", &debug->drawSeparationForce);

          EditSFMLColor("Separation Force Color", debug->separationForceColor);

          ImGui::Checkbox("Draw Final Steering Force", &debug->drawFinalSteeringForce);

          EditSFMLColor("Final Steering Force Color", debug->finalSteeringForceColor);

          ImGui::EndDisabled();
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

    // Entidad cuyo Texture Path se encuentra cargado actualmente en el buffer del Inspector.
    ECS::EntityID m_textureBufferEntity{ECS::NULL_ENTITY};

    // Ruta editable antes de intentar cargarla.
    std::array<char, 512> m_texturePathBuffer{};

    // Mensaje temporal del resultado de carga.
    bool m_textureLoadSucceeded{ false };
    bool m_textureLoadFailed{ false };
    float m_textureMessageTimer{ 0.f };

    /**
      * @brief Edita un sf::Color mediante ImGui.
      *
      * Convierte temporalmente los canales de 0-255 utilizados
      * por SFML al rango 0-1 utilizado por ImGui.
      */
    void 
    EditSFMLColor(const char* label, sf::Color& color)
    {
      float values[4] = {
          static_cast<float>(color.r) / 255.f,
          static_cast<float>(color.g) / 255.f,
          static_cast<float>(color.b) / 255.f,
          static_cast<float>(color.a) / 255.f
      };

      if (ImGui::ColorEdit4(label, values))
      {
        color.r = static_cast<std::uint8_t>(values[0] * 255.f);
        color.g = static_cast<std::uint8_t>(values[1] * 255.f);
        color.b = static_cast<std::uint8_t>(values[2] * 255.f);
        color.a = static_cast<std::uint8_t>(values[3] * 255.f);
      }
    }

    /**
      * @brief Devuelve el nombre visible de un ShapeType.
      */
    [[nodiscard]] const char*
    ShapeTypeLabel(ShapeType type) const noexcept
    {
      switch (type)
      {
      case EMPTY:
        return "Empty";

      case CIRCLE:
        return "Circle";

      case RECTANGLE:
        return "Rectangle";

      case TRIANGLE:
        return "Triangle";

      case POLYGON:
        return "Polygon";

      case LINE:
        return "Line";

      default:
        return "Unknown";
      }
    }

    /**
      * @brief Sincroniza el buffer de ruta cuando cambia
      * la entidad seleccionada.
      */
    void 
    SyncTexturePathBuffer(EntityID entity, const Render& render)
    {
      if (m_textureBufferEntity == entity)return;

      m_textureBufferEntity =entity;

      m_texturePathBuffer.fill('\0');

      std::snprintf(m_texturePathBuffer.data(),m_texturePathBuffer.size(),"%s",
                    render.texturePath.c_str());

      m_textureLoadSucceeded = false;

      m_textureLoadFailed = false;

      m_textureMessageTimer = 0.f;
    }

    /**
      * @brief Muestra los controles geométricos específicos del ShapeType actual.
      */
    void 
    DrawShapeDimensions(Render& render)
    {
      if (!render.shape)
      {
        ImGui::TextDisabled("This Render has no drawable shape.");
        return;
      }

      switch (render.shapeType)
      {
      case CIRCLE:
      {
        auto circle = std::dynamic_pointer_cast<sf::CircleShape>(render.shape);

        if (!circle)
        {
          ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), "Invalid CircleShape instance.");
          return;
        }

        float radius = circle->getRadius();

        if (ImGui::DragFloat("Radius", &radius, 0.5f, 1.f, 2000.f))
        {
          circle->setRadius(radius);

          circle->setOrigin({radius,radius});
        }
        break;
      }

      case RECTANGLE:
      {
        auto rectangle =std::dynamic_pointer_cast<sf::RectangleShape>(render.shape);

        if (!rectangle)
        {
          ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f),"Invalid RectangleShape instance.");
          return;
        }

        sf::Vector2f size = rectangle->getSize();

        if (ImGui::DragFloat2("Size", &size.x, 1.f, 1.f, 4000.f))
        {
          size.x = std::max(1.f, size.x);
          size.y = std::max(1.f, size.y);

          rectangle->setSize(size);

          rectangle->setOrigin(size / 2.f);
        }
        break;
      }

      case LINE:
      {
        auto line = std::dynamic_pointer_cast<sf::RectangleShape>(render.shape);

        if (!line)
        {
          ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), "Invalid Line shape instance.");
          return;
        }

        sf::Vector2f size = line->getSize();

        float length =size.x;
        float thickness =size.y;

        bool changed = false;

        changed |= ImGui::DragFloat("Length", &length, 1.f, 1.f, 5000.f);

        changed |= ImGui::DragFloat("Thickness", &thickness, 0.25f, 1.f, 500.f);

        if (changed)
        {
          length =std::max(1.f, length);
          thickness =std::max(1.f, thickness);

          line->setSize({length, thickness});

          // La línea comienza en Transform.position
          // y se extiende hacia su eje X local.
          line->setOrigin({0.f, thickness / 2.f});
        }
        break;
      }

      case TRIANGLE:
      {
        auto triangle =std::dynamic_pointer_cast<sf::ConvexShape>(render.shape);

        if (!triangle ||triangle->getPointCount() != 3)
        {
          ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), "Invalid Triangle shape instance.");
          return;
        }

        float width = triangle->getPoint(1).x;
        float height = triangle->getPoint(2).y;

        bool changed = false;

        changed |= ImGui::DragFloat("Width", &width, 1.f, 1.f, 4000.f);

        changed |= ImGui::DragFloat("Height", &height, 1.f, 1.f, 4000.f);

        if (changed)
        {
          width = std::max(1.f, width);
          height = std::max(1.f, height);

          triangle->setPoint(0, { 0.f, 0.f });
          triangle->setPoint(1, { width, 0.f });
          triangle->setPoint(2, {width / 2.f, height});

          triangle->setOrigin({width / 2.f, height / 2.f});
        }
        break;
      }

      case POLYGON:
      {
        auto polygon =std::dynamic_pointer_cast<sf::ConvexShape>(render.shape);

        if (!polygon || polygon->getPointCount() != 5)
        {
          ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), "Invalid Polygon shape instance.");
          return;
        }

        // La geometría predeterminada utiliza el punto
        // derecho como ancho máximo y el inferior como alto.
        float width = polygon->getPoint(2).x;
        float height = polygon->getPoint(3).y;

        bool changed = false;

        changed |= ImGui::DragFloat("Width", &width, 1.f, 1.f, 4000.f);
        changed |= ImGui::DragFloat("Height", &height, 1.f, 1.f, 4000.f);

        if (changed)
        {
          width = std::max(1.f, width);
          height = std::max(1.f, height);

          polygon->setPoint(0, {0.f, height * 0.5f});
          polygon->setPoint(1, {width * 0.2f, 0.f});
          polygon->setPoint(2, {width * 0.8f, 0.f});
          polygon->setPoint(3, {width, height * 0.5f});
          polygon->setPoint(4, {width * 0.5f, height });

          polygon->setOrigin({width / 2.f, height / 2.f});
        }
        break;
      }

      case EMPTY:
      {
        ImGui::TextDisabled("Select a Shape Type to create geometry.");
        break;
      }

      default:
        break;
      }
    }

    /**
      * @brief Dibuja todos los controles del componente Render.
      */
    void 
    DrawRenderInspector(EntityID entity,Render& render)
    {
      SyncTexturePathBuffer(entity,render);

      // --------------------------------------------------
      // General
      // --------------------------------------------------

      ImGui::Checkbox("Visible", &render.visible);
      ImGui::DragInt("Z Order", &render.zOrder, 1.f, -10000, 10000);

      // --------------------------------------------------
      // Shape Type
      // --------------------------------------------------

      const char* currentShapeLabel = ShapeTypeLabel(render.shapeType);

      if (ImGui::BeginCombo("Shape Type", currentShapeLabel))
      {
        constexpr ShapeType shapeTypes[] = {
            EMPTY,
            CIRCLE,
            RECTANGLE,
            TRIANGLE,
            POLYGON,
            LINE
        };

        for (const ShapeType candidate : shapeTypes)
        {
          const bool selected = candidate == render.shapeType;

          if (ImGui::Selectable(ShapeTypeLabel(candidate), selected))
          {
            render.RebuildShape(candidate);
          }

          if (selected) ImGui::SetItemDefaultFocus();
        }

        ImGui::EndCombo();
      }

      // --------------------------------------------------
      // Shape dimensions
      // --------------------------------------------------

      ImGui::Separator();
      ImGui::TextUnformatted("Geometry");

      DrawShapeDimensions(render);

      // --------------------------------------------------
      // Fill Color
      // --------------------------------------------------

      ImGui::Separator();
      ImGui::TextUnformatted("Appearance");

      EditSFMLColor("Fill Color",render.fillColor);

      if (render.shape) render.shape->setFillColor(render.fillColor);

      // --------------------------------------------------
      // Texture
      // --------------------------------------------------

      ImGui::Separator();
      ImGui::TextUnformatted("Texture");

      ImGui::InputText("Texture Path", m_texturePathBuffer.data(), m_texturePathBuffer.size());

      const bool canLoadTexture = render.shape != nullptr && m_texturePathBuffer[0] != '\0';

      ImGui::BeginDisabled(!canLoadTexture);

      if (ImGui::Button("Load Texture"))
      {
        const std::string requestedPath{m_texturePathBuffer.data()};

        const bool loaded = render.SetTexture(requestedPath);

        m_textureLoadSucceeded = loaded;
        m_textureLoadFailed = !loaded;
        m_textureMessageTimer =3.f;
      }

      ImGui::EndDisabled();

      ImGui::SameLine();

      ImGui::BeginDisabled(!render.texture);

      if (ImGui::Button("Clear Texture"))
      {
        render.ClearTexture();

        m_texturePathBuffer.fill('\0');
        m_textureLoadSucceeded = false;
        m_textureLoadFailed = false;
        m_textureMessageTimer = 0.f;
      }

      ImGui::EndDisabled();

      if (render.texture)
      {
        ImGui::TextColored(ImVec4(0.3f, 1.f, 0.5f, 1.f), "Texture loaded");

        if (!render.texturePath.empty())
        {
          ImGui::TextWrapped("%s", render.texturePath.c_str());
        }
      }
      else
      {
        ImGui::TextDisabled("No texture loaded");
      }

      if (m_textureMessageTimer > 0.f)
      {
        if (m_textureLoadSucceeded)
        {
          ImGui::TextColored(ImVec4(0.3f, 1.f, 0.5f, 1.f), "Texture loaded successfully.");
        }

        if (m_textureLoadFailed)
        {
          ImGui::TextColored(ImVec4(1.f, 0.3f, 0.3f, 1.f), "Failed to load texture.");
        }
      }
    }

    /**
      * @brief Genera un nombre único para una nueva entidad.
      *
      * Devuelve "New Entity" si el nombre está disponible.
      * Si ya existe, prueba "New Entity (1)", "New Entity (2)", etc.
      *
      * @param registry Registry utilizado para consultar entidades vivas.
      * @return Nombre único para mostrar en la Hierarchy.
      */
    [[nodiscard]] std::string
    GenerateUniqueEntityName(Registry& registry) const {
      constexpr const char* baseName = "New Entity";

      bool baseNameExists = false;

      for (const EntityID entity : registry.GetAllEntities())
      {
        if (!registry.IsAlive(entity)) continue;

        const Name* name = registry.TryGetComponent<Name>(entity);

        if (name && name->name == baseName)
        {
          baseNameExists = true;
          break;
        }
      }

      if (!baseNameExists) return baseName;

      int suffix = 1;

      while (true)
      {
        const std::string candidate = std::string(baseName) + " (" + std::to_string(suffix) + ")";

        bool candidateExists = false;

        for (const EntityID entity : registry.GetAllEntities())
        {
          if (!registry.IsAlive(entity)) continue;

          const Name* name = registry.TryGetComponent<Name>(entity);

          if (name && name->name == candidate)
          {
            candidateExists = true;
            break;
          }
        }

        if (!candidateExists) return candidate;

        ++suffix;
      }
    }

    [[nodiscard]] const char*
    PathEditModeLabel(PathEditMode mode) const noexcept
    {
      switch (mode)
      {
      case PathEditMode::None:
        return "None";

      case PathEditMode::Add:
        return "Add Points";

      case PathEditMode::Move:
        return "Move Points";

      case PathEditMode::Delete:
        return "Delete Points";

      default:
        return "Unknown";
      }
    }

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

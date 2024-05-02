#include "Guizmo.h"
#include "Core/Singleton.h"
#include "Core/Editor/Editor.h"
#include <glm.hpp>
#include "Dependencies/GLM/gtc/type_ptr.hpp"
#include <Core/ParticleSystem/FireSystem.h>




void Guizmo::Render(Core::Primitives::Camera* camera)
{
	cam = camera;
	Draw();
}

void Guizmo::Draw()
{
	SelectedObj& selectedObjIns = Singleton<Editor>::Instance().GetSelectedObj();
	ImGuizmo::OPERATION curMode = *Singleton<Editor>::Instance().GetGuizmoMode();


	auto meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(selectedObjIns.GetSelectedComponent());
	auto fireSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(selectedObjIns.GetSelectedComponent());

	
	ImGuizmo::AllowAxisFlip(false);
	ImGuizmo::SetOrthographic(false);

	glm::mat4 camView = cam->GetViewMatrix();
	glm::mat4 camProjection = cam->GetProjectionMatrix();

	glm::mat4 modelMatrix = glm::mat4(1.0f); //init como matriz identidad
	
	//ImGuizmo::ViewManipulate(glm::value_ptr(camView), 100, ImVec2(ImGui::GetWindowPos().x + (float)ImGui::GetWindowWidth() , ImGui::GetWindowPos().y), ImVec2(128, 128), 0x10101010);

	if (selectedObjIns.GetSelectedObject() && !fireSystem) {

		// se aplica transformación pos, rot y escala
		modelMatrix = glm::translate(modelMatrix, selectedObjIns.GetSelectedObject()->GetPosition()); 
		modelMatrix = glm::rotate(modelMatrix, glm::radians(selectedObjIns.GetSelectedObject()->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)); // x
		modelMatrix = glm::rotate(modelMatrix, glm::radians(selectedObjIns.GetSelectedObject()->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)); // y
		modelMatrix = glm::rotate(modelMatrix, glm::radians(selectedObjIns.GetSelectedObject()->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f)); // z
		modelMatrix = glm::scale(modelMatrix, selectedObjIns.GetSelectedObject()->GetScale()); 

		ImGuizmo::Manipulate(
			glm::value_ptr(camView), 
			glm::value_ptr(camProjection),
			curMode,
			ImGuizmo::LOCAL, 
			glm::value_ptr(modelMatrix)
		);

		if (ImGuizmo::IsUsing()) {
			Singleton<Editor>::Instance().SetEditorLocked(true);

			glm::vec3 position = glm::vec3(modelMatrix[3]);
			glm::vec3 rotation = glm::eulerAngles(glm::quat_cast(modelMatrix));
			glm::vec3 scale = glm::vec3(
				glm::length(glm::vec3(modelMatrix[0])),
				glm::length(glm::vec3(modelMatrix[1])),
				glm::length(glm::vec3(modelMatrix[2]))
			);

			selectedObjIns.GetSelectedObject()->SetPosition(position);
			selectedObjIns.GetSelectedObject()->SetRotation(rotation);
			selectedObjIns.GetSelectedObject()->SetScale(scale);
		}

	}
	else if (selectedObjIns.GetSelectedComponent()){
		//otro diferente pal fire poruqe a ling le gusta el obo

		std::shared_ptr<Core::Component> particleComp = selectedObjIns.GetSelectedComponent();
		std::shared_ptr<Core::Particles::FireSystem> particleSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(particleComp);

		modelMatrix = glm::translate(modelMatrix, particleSystem->GetSystemCenter());
		//no rotation for fire
		modelMatrix = glm::scale(modelMatrix, particleSystem->GetRadiusVector());

		ImGuizmo::Manipulate(
			glm::value_ptr(camView),
			glm::value_ptr(camProjection),
			curMode,
			ImGuizmo::LOCAL,
			glm::value_ptr(modelMatrix)
		);

		
		if (ImGuizmo::IsUsing()) {
			Singleton<Editor>::Instance().SetEditorLocked(true);

			glm::vec3 position = glm::vec3(modelMatrix[3]);
			glm::vec3 rotation = glm::eulerAngles(glm::quat_cast(modelMatrix));
			glm::vec3 scale = glm::vec3(
				glm::length(glm::vec3(modelMatrix[0])),
				glm::length(glm::vec3(modelMatrix[1])),
				glm::length(glm::vec3(modelMatrix[2]))
			);

			particleSystem->SetSystemCenter(position, selectedObjIns.GetSelectedObject()->GetPosition());
			fireSystem->ChangeFireSize(scale[0], scale[1], scale[2], fireSystem->GetFireGap(), fireSystem->GetFireHeight());
		}
	
	}
	
}
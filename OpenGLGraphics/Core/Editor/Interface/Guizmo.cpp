#include "Guizmo.h"
#include "Core/Singleton.h"
#include "Core/Editor/Editor.h"
#include <glm.hpp>
#include "Dependencies/GLM/gtc/type_ptr.hpp"




void Guizmo::Render(Core::Primitives::Camera* camera)
{
	cam = camera;
	Draw();
}

void Guizmo::Draw()
{
	SelectedObj& selectedObjIns = Singleton<Editor>::Instance().GetSelectedObj();

	if (selectedObjIns.GetSelectedObject()) {
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist();
		float windowWidth = (float)ImGui::GetWindowWidth();
		float windowHeight = (float)ImGui::GetWindowHeight();

		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth,windowHeight);

		glm::mat4 camView = cam->GetViewMatrix();

		glm::mat4 camPojection = cam->GetProjectionMatrix();

		selectedObjIns.GetSelectedObject()->GetPosition();

		glm::mat4 transform ;

		

		ImGuizmo::Manipulate(
			glm::value_ptr(camView), 
			glm::value_ptr(camPojection), 
			ImGuizmo::OPERATION::TRANSLATE, 
			ImGuizmo::LOCAL, 
			glm::value_ptr(transform)
		);


	}
	
}
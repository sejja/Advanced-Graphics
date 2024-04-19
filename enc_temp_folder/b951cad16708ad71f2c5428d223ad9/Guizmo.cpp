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
		ImGuizmo::AllowAxisFlip(false);
		ImGuizmo::SetOrthographic(false);

		//ImGuizmo::SetDrawlist(Singleton<Editor>::Instance().GetWindowDrawList());
	


		//glm::lowp_u16vec2 windowSize = Singleton<Editor>::Instance().GetSceneFrameDimensions();
		//ImVec2 scenePosition = Singleton<Editor>::Instance().GetSceneFramePosition();

		

		glm::mat4 camView = cam->GetViewMatrix();
		glm::mat4 camProjection = cam->GetProjectionMatrix();

		glm::mat4 modelMatrix = glm::mat4(1.0f); //init como matriz identidad

		// se aplica transformación pos, rot y escala
		modelMatrix = glm::translate(modelMatrix, selectedObjIns.GetSelectedObject()->GetPosition()); 
		modelMatrix = glm::rotate(modelMatrix, glm::radians(selectedObjIns.GetSelectedObject()->GetRotation().x), glm::vec3(1.0f, 0.0f, 0.0f)); // x
		modelMatrix = glm::rotate(modelMatrix, glm::radians(selectedObjIns.GetSelectedObject()->GetRotation().y), glm::vec3(0.0f, 1.0f, 0.0f)); // y
		modelMatrix = glm::rotate(modelMatrix, glm::radians(selectedObjIns.GetSelectedObject()->GetRotation().z), glm::vec3(0.0f, 0.0f, 1.0f)); // z
		modelMatrix = glm::scale(modelMatrix, selectedObjIns.GetSelectedObject()->GetScale()); 

		ImGuizmo::Manipulate(
			glm::value_ptr(camView), 
			glm::value_ptr(camProjection),
			ImGuizmo::OPERATION::ROTATE, 
			ImGuizmo::LOCAL, 
			glm::value_ptr(modelMatrix)
		);

		if (ImGuizmo::IsUsing()) {
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
	
}
#include "Editor.h"
#include <iostream>

Editor::Editor() : editorLocked(false) {}

bool Editor::isEditorLocked() {
	return editorLocked;
}

void Editor::render(Core::Graphics::OpenGLPipeline& pipeline){

	char buf[256]; 
	float f = 0.0f; 

	strcpy(buf, "test");

	ImGui::Text("YEYEYE", 123);
	if (ImGui::Button("Save"))
		printf("TEST");
	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	editorLocked = ImGui::IsAnyItemActive();

	ImGui::Begin("Scene");
	ImGui::Text("YEYEYE", 123);
	if (ImGui::Button("Save"))
		printf("TEST");
	ImGui::End();

	
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);



	


	
	if (ImGui::Begin("Deferred Rendering")) {
		ImGui::Image((ImTextureID)pipeline.GetGBuffer() ->GetPositionTextureHandle(),
			ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Image((ImTextureID)pipeline.GetGBuffer()->GetAlbedoTextureHandle(),
			ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)pipeline.GetGBuffer()->GetNormalTextureHandle(),
			ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
	


	/*
	if (ImGui::Begin("Shadow Mapping")) {
		int i = 0;
		for (FrameBuffer& buff : mShadowBuffers) {
			ImGui::Image((ImTextureID)buff.GetTextureHandle(),
				ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

			if (i < 1) {
				ImGui::SameLine();
				i++;
			}
			else
				i = 0;
		}
	}
	ImGui::End();
	*/
}


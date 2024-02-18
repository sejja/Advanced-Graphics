#include "Editor.h"
#include <iostream>
#include "Interface/Properties.h"

Editor::Editor() : editorLocked(false) {}

bool Editor::IsEditorLocked() {
	return editorLocked;
}

void Editor::Render(Core::Graphics::OpenGLPipeline& pipeline){

	//If any input is being used, camera controls are locked
	editorLocked = ImGui::IsAnyItemActive();

	//Abre una demo de opciones de imgui
	ImGui::ShowDemoWindow();


	properties.Render();
	outliner.Render();

	ImGui::Begin("Scene");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	// add rendered texture to ImGUI scene window
	uint64_t textureID = pipeline.GetGBuffer()->GetNormalTextureHandle();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });


	ImGui::End();


}


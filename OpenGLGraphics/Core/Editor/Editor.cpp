#include "Editor.h"
#include <iostream>
#include "Interface/Properties.h"
#include "Interface/AssetManager.h"
#include "Graphics/Tools/OpenGLInfo.h"

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
	AssetManager assetManager;
	assetManager.Render();
	outliner.Render();
	

	
	ImGui::Begin("Scene");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	//habr�a que setear la vista de la camara de la escena a este viewport tmb
	uint64_t textureID = pipeline.GetRenderFrameBuffer()->GetTextureHandle();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	

	
	ImGui::Begin("Scene");
	viewportPanelSize = ImGui::GetContentRegionAvail();
	//habr�a que setear la vista de la camara de la escena a este viewport tmb
	textureID = pipeline.GetRenderFrameBuffer()->GetTextureHandle();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();
	auto size = ImGui::GetWindowSize();

	Singleton<Core::Graphics::OpenGLInfo>::Instance().mDimensions = glm::vec2(size.x *5, size.y*4);
	


}


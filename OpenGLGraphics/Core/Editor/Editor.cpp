#include "Editor.h"
#include <iostream>
#include "Interface/Properties.h"
#include "Interface/AssetManager.h"
#include "Core/Window/SDLWindow.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Singleton.h"

Editor::Editor() : editorLocked(false) {}

bool Editor::IsEditorLocked() {
	return editorLocked;
}

void Editor::Render(Core::Graphics::OpenGLPipeline& pipeline){

	//If any input is being used, camera controls are locked
	editorLocked = ImGui::IsAnyItemActive();

	//Abre una demo de opciones de imgui
	ImGui::ShowDemoWindow();


	//Singleton para gestionar objeto seleccionado en cualquier 
	Singleton<SelectedObj>::Instance();





	//AssetManager assetManager;
	assetManager.Render();
	properties.Render();
	outliner.Render();
	

	
	ImGui::Begin("Scene");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	//habría que setear la vista de la camara de la escena a este viewport tmb

	glm::lowp_u16vec2 dim;
	dim.x = static_cast<uint16_t>(viewportPanelSize.x);
	dim.y = static_cast<uint16_t>(viewportPanelSize.y);
	Singleton<SDLWindow>::Instance().SetDimensions(dim);
	
	uint64_t textureID = pipeline.GetRenderFrameBuffer()->GetTextureHandle();
	ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();

	


}


#include "SceneView.h"
#include <iostream>
#include "Core/Window/SDLWindow.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Singleton.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"


void SceneView::Render(Core::Graphics::OpenGLPipeline& pipeline){

	ImGui::Begin(ICON_FA_VIDEO " Scene");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	//habr�a que setear la vista de la camara de la escena a este viewport tmb

	glm::lowp_u16vec2 dim;
	dim.x = static_cast<uint16_t>(viewportPanelSize.x);
	dim.y = static_cast<uint16_t>(viewportPanelSize.y);

	//printf("Viewport size 1: %d, %d\n", dim.x, dim.y);

	pipeline.setSceneFrameDimensions(dim);
	//pipeline.Render();
	ImGui::Image(reinterpret_cast<void*>(pipeline.GetRenderTexture()), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();


}
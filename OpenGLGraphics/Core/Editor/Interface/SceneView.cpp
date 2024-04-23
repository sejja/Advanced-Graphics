#include "SceneView.h"
#include <iostream>
#include "Core/Window/SDLWindow.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Singleton.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include "Core/Editor/Editor.h"
#include "Core/Input/InputManager.h"
#include "Core/Editor/mouse_picking.h"



void SceneView::Render(Core::Graphics::OpenGLPipeline& pipeline){

	ImGui::Begin(ICON_FA_VIDEO " Scene");
	ImGuizmo::BeginFrame();
	ImGuizmo::SetDrawlist();

	Core::Input::InputManager& inptmgr = Singleton<Core::Input::InputManager>::Instance();
	ImVec2 screen_pos = ImGui::GetCursorScreenPos();
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	glm::vec2 mousePosition(inptmgr.GetMousePosition().x - screen_pos.x, inptmgr.GetMousePosition().y - screen_pos.y);

	glm::lowp_u16vec2 dim;
	dim.x = static_cast<uint16_t>(viewportPanelSize.x);
	dim.y = static_cast<uint16_t>(viewportPanelSize.y);


	ImGuizmo::SetRect(
		screen_pos.x, screen_pos.y,
		viewportPanelSize.x, viewportPanelSize.y
	);

	Singleton<Editor>::Instance().setSceneFrameDimensions(dim);

	Singleton<Editor>::Instance().setSceneFramePosition(screen_pos);

	//picking
	mousePicking.performRayCasting(mousePosition, viewportPanelSize.x, viewportPanelSize.y, pipeline.getCamera());


	
	ImGui::Image(reinterpret_cast<void*>(pipeline.GetRenderTexture()), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
	ImGui::End();


}

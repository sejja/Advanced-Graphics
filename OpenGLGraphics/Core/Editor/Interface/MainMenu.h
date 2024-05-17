
#ifndef __MAINMENU__H__
#define __MAINMENU__H__

#include "Dependencies/ImGui/imgui.h"
#include "Graphics/OpenGLPipeline.h"
#include "Core/Editor/Interface/DebugTools.h"


class MainMenu {
public:
	void Render(Core::Graphics::OpenGLPipeline& pipeline);
private:
	DebugTools debugTools;

	void RenderFileMenu();
	void RenderRemoteControlMenu();
	void RenderEditMenu();
	void RenderServerStateInfo();
	void RenderActionButtons();
	void RenderIsSavingState();
	void RenderSceneLoader();

	bool savePopup = false;
	bool previouslySaved = false;
	char saveFileName[100] = "";


};

#endif
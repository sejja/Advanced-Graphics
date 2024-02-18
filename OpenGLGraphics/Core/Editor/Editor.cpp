#include "Editor.h"
#include <iostream>
#include "Interface/Properties.h"
#include "Interface/AssetManager.h"

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
	


}


#include "Editor.h"
#include <iostream>
#include "Interface/Properties.h"

Editor::Editor() : editorLocked(false) {}

bool Editor::IsEditorLocked() {
	return editorLocked;
}

void Editor::Render(Core::Graphics::OpenGLPipeline& pipeline){

	//If any input is being used 
	editorLocked = ImGui::IsAnyItemActive();

	//Abre una demo de opciones de imgui
	ImGui::ShowDemoWindow();


	properties.Render();


}


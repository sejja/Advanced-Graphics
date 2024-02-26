#include "Editor.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include "Interface/Properties.h"
#include "Interface/AssetManager.h"
#include "Core/Window/SDLWindow.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Singleton.h"

#include "Interface/TextEditor.h"

Editor::Editor() : editorLocked(false) {}

bool Editor::IsEditorLocked() {
    return editorLocked;
}

void Editor::Render(Core::Graphics::OpenGLPipeline& pipeline) {

    //If any input is being used, camera controls are locked
    editorLocked = ImGui::IsAnyItemActive();

    //Abre una demo de opciones de imgui
    ImGui::ShowDemoWindow();

	//Singleton para gestionar objeto seleccionado
	Singleton<SelectedObj>::Instance();

	mainMenu.Render(pipeline);
	assetManager.Render();
	properties.Render();
	outliner.Render();

	//auto lang = TextEditor::LanguageDefinition::Lua();
	//textEditor.SetLanguageDefinition(lang);
	ImGui::Begin("Text Editor");
	ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
	textEditor.Render("Patata", ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, false);

	//SceneView
	sceneView.Render(pipeline);
	

	


	


}


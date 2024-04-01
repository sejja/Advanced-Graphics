#include "Editor.h"
#include <iostream>
#include "Interface/AssetManager.h"
#include "Core/Window/SDLWindow.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Singleton.h"
#include "database.h"



Editor::Editor() : editorLocked(false) {
	this->database = new Core::Editor::Database("../AssetLoader/database.db");
	//assetManager = std::make_unique < AssetManager >();
}

bool Editor::IsEditorLocked() {
	return editorLocked;
}

void Editor::SetEditorLocked(bool locked) {
	editorLocked = locked;
}

void Editor::Render(Core::Graphics::OpenGLPipeline& pipeline){

	//If any input is being used, camera controls are locked
	editorLocked = ImGui::IsAnyItemActive() || texteditor.isFocused();

	//Abre una demo de opciones de imgui
	ImGui::ShowDemoWindow();


	mainMenu.Render(pipeline);
	assetManager.Render();
	properties.Render();
	outliner.Render();


	//SceneView
	sceneView.Render(pipeline);
	
	texteditor.Render("TextEditor",1);

	


	


}

Editor::~Editor() {
	database->closeConnection();
	delete database;
}


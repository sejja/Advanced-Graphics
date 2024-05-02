#include "Editor.h"
#include <iostream>
#include "Interface/AssetManager.h"
#include "Core/Window/SDLWindow.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Singleton.h"
#include "database.h"
#include "Core/Editor/Interface/AssetIcon.h"



Editor::Editor() : editorLocked(false) {
	this->database = new Core::Editor::Database("../AssetLoader/database.db");
	database->getAssetTypeImages();
	AssetIcon::assetTypeImages = database->getAssetTypeImages();
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

	//Singleton para gestionar objeto seleccionado
	Singleton<SelectedObj>::Instance();

	mainMenu.Render(pipeline);
	assetManager.Render();
	properties.Render(pipeline);
	outliner.Render();


	//SceneView
	sceneView.Render(pipeline);
	
	texteditor.Render("Shader Editor",1);

	


	


}

Editor::~Editor() {
	database->closeConnection();
	delete database;
}


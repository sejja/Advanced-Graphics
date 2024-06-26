#include "Editor.h"
#include <iostream>
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

bool Editor::IsSceneHovered() {
	return sceneView.isHovered();
}

void Editor::SetEditorLocked(bool locked) {
	editorLocked = locked;
}

void Editor::Render(Core::Graphics::OpenGLPipeline& pipeline){

	//If any input is being used, camera controls are locked
	editorLocked = properties.isFocused() || texteditor.isFocused();
	
	

	mainMenu.Render(pipeline);
	assetManager.Render();
	properties.Render(pipeline);
	outliner.Render();


	
	texteditor.Render("Shader Editor",1);

	//SceneView
	sceneView.Render(pipeline);

	//Guizmo
	guizmo.Render(pipeline.getCamera());

	//Keys
	keyBinder.CheckKeyBindings();

	

	



	


}

Editor::~Editor() {
	database->closeConnection();
	delete database;
}


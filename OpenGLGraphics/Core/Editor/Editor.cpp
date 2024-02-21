#include "Editor.h"
#include <iostream>
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

	//SceneView sceneView;
	sceneView.Render(pipeline);
	

	


	


}


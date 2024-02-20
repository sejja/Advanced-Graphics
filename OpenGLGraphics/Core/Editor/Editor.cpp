#include "Editor.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include "Interface/Properties.h"
#include "Interface/AssetManager.h"
#include "Interface/TextEditor.h"

Editor::Editor() : editorLocked(false) {}
bool firstTime = true;


bool Editor::IsEditorLocked() {
    return editorLocked;
}

void Editor::Render(Core::Graphics::OpenGLPipeline& pipeline) {

    //If any input is being used, camera controls are locked
    editorLocked = ImGui::IsAnyItemActive();

    //Abre una demo de opciones de imgui
    ImGui::ShowDemoWindow();

    AssetManager assetManager;
    static TextEditor textEditor;
    assetManager.Render();
    properties.Render();
    outliner.Render();

    ImGui::Begin("Text Editor");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    textEditor.Render("Patata", ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, false);
    static const char* fileToEdit = "Core/Editor/patata.txt";
    { // Save
		if (ImGui::Button("Save")) {
			std::ofstream t(fileToEdit);
			t << textEditor.GetText();
		}
	}
    std::ifstream t(fileToEdit);
    //std::cout << "Error: " << strerror(errno) << std::endl;
    if (t.good() & firstTime)
    {
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        textEditor.SetText(str);
        firstTime = false;
    }
    ImGui::End();
    
    ImGui::Begin("Scene");
    viewportPanelSize = ImGui::GetContentRegionAvail();
    //habría que setear la vista de la camara de la escena a este viewport tmb
    uint64_t textureID = pipeline.GetRenderFrameBuffer()->GetTextureHandle();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
}

void editamos() {
	std::cout << "Editamos" << std::endl;
}
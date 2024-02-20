#include "Editor.h"
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include "Interface/Properties.h"
#include "Interface/AssetManager.h"
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

    AssetManager assetManager;
    static TextEditor textEditor;
    assetManager.Render();
    properties.Render();
    outliner.Render();
    textEditor.SetHandleKeyboardInputs(true);
    char text[100] = "patata";
    ImGui::Begin("Text Editor");

    textEditor.SetReadOnly(false);
    //textEditor.SetText(text);
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    textEditor.Render("Patata", ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, false);
    static const char* fileToEdit = "patata.txt";
    //	static const char* fileToEdit = "test.cpp";

    {
        std::ifstream t(fileToEdit);
        if (t.good())
        {
            std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
            textEditor.SetText(str);
        }
    }
    ImGui::End();
    
    ImGui::Begin("Scene");
    viewportPanelSize = ImGui::GetContentRegionAvail();
    //habría que setear la vista de la camara de la escena a este viewport tmb
    uint64_t textureID = pipeline.GetRenderFrameBuffer()->GetTextureHandle();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
}

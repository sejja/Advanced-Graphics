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
    assetManager.Render();
    properties.Render();
    outliner.Render();
    //auto lang = TextEditor::LanguageDefinition::Lua();
    //textEditor.SetLanguageDefinition(lang);
    ImGui::Begin("Text Editor");
    ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
    textEditor.Render("Patata", ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, false);
    
    ImGui::Begin("Scene");
    viewportPanelSize = ImGui::GetContentRegionAvail();
    //habría que setear la vista de la camara de la escena a este viewport tmb
    uint64_t textureID = pipeline.GetRenderFrameBuffer()->GetTextureHandle();
    ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportPanelSize.x, viewportPanelSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
    ImGui::End();
}
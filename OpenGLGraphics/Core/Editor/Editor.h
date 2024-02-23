#ifndef __EDITOR__H__
#define __EDITOR__H__

#include <unordered_map>
#include <vector>
#include <memory>
#include "Dependencies/ImGui/imgui.h"
#include "Graphics/OpenGLPipeline.h"
#include "Interface/Properties.h"
#include "Interface/Outliner.h"
#include "Interface/AssetManager.h"
#include "Interface/SceneView.h"
#include "Interface/MainMenu.h"



class Editor {
public:
    Editor();

    void Render(Core::Graphics::OpenGLPipeline& pipeline);

    bool IsEditorLocked();

private:
    MainMenu mainMenu;
    Properties properties;
    AssetManager assetManager;
    Outliner outliner;
    SceneView sceneView;
    bool editorLocked;


};


#endif 
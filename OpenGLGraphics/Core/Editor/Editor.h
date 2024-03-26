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
#include "Core/Editor/SelectedObj.h"
#include "database.h"
#include "Core/Editor/Interface/TextEditor.h"



class Editor {

public:
    Editor();
    ~Editor();

    void Render(Core::Graphics::OpenGLPipeline& pipeline);

    bool IsEditorLocked();
    void SetEditorLocked(bool locked);
    Core::Editor::Database* database;
    AssetManager assetManager;
    TextEditor texteditor;

    SelectedObj& GetSelectedObj() { return selectedObj; }
    

private:
    bool editorLocked;
    SelectedObj selectedObj;

    MainMenu mainMenu;
    Properties properties;
    
    Outliner outliner;
    SceneView sceneView;

    
};


#endif 
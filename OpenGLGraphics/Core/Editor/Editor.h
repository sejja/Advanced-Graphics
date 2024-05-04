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
#include "Guizmo.h"
#include "Core/Editor/SelectedObj.h"
#include "database.h"
#include "Core/Editor/Interface/TextEditor.h"

#include "Core/Editor/ActionManager/ActionManager.h"


class Editor {

public:
    Editor();
    ~Editor();

    void Render(Core::Graphics::OpenGLPipeline& pipeline);

    bool IsEditorLocked();
	bool IsSceneHovered();
    void SetEditorLocked(bool locked);
    Core::Editor::Database* database;
    AssetManager assetManager;
    TextEditor texteditor;

    SelectedObj& GetSelectedObj() { return selectedObj; }

    void setSceneFrameDimensions(const glm::lowp_u16vec2& dim) { sceneFrameDimensions = dim; }
    float GetAspectRatio() { return static_cast<float>(sceneFrameDimensions.x) / static_cast<float>(sceneFrameDimensions.y); }
    glm::lowp_u16vec2 GetSceneFrameDimensions() { return sceneFrameDimensions; }
    
    void setSceneFramePosition(const ImVec2& pos) { sceneFramePosition = pos; }
    ImVec2 GetSceneFramePosition() { return sceneFramePosition; }


    float *GetFOV() { return &fov; }

    void SetGuizmoMode(ImGuizmo::OPERATION mode) { guizmoMode = mode; }
    ImGuizmo::OPERATION* GetGuizmoMode() { return &guizmoMode; }

	ActionManager* GetActionManager() { return &actionManager; }

	bool getIsEditing() { return editComplete; }
	void setEditing(bool value) { editComplete = value; }
    

private:
    SelectedObj selectedObj;
    MainMenu mainMenu;
    Properties properties;
    Outliner outliner;
    SceneView sceneView;
    Guizmo guizmo;


    glm::lowp_u16vec2 sceneFrameDimensions;
    ImVec2 sceneFramePosition;
    ImGuizmo::OPERATION guizmoMode = ImGuizmo::OPERATION::TRANSLATE;


    ImDrawList* drawList;

	ActionManager actionManager;



    float fov = 45.0f;
    bool editorLocked;

	bool editComplete = false;

};


#endif 
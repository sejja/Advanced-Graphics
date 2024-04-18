#include "Outliner.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/Assets/ResourceManager.h"
#include "Core/ECSystem/Scene.h"
#include "Core/AppWrapper.h"
#include "Core/ECSystem/Scene.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include "Core/Editor/Editor.h"


SelectedObj& selectedObj = Singleton<Editor>::Instance().GetSelectedObj();


void Outliner::Render(){
	ImGui::Begin(ICON_FA_FOLDER "  Outliner");

	RenderOptions();
	RenderSceneObjects();

	ImGui::End();

}



std::string Outliner::formatObjName(const std::shared_ptr<Core::Object>& obj) {
	std::string displayName = obj->GetName();

	// de momento así por que los objetos de la escena no tienen un tipo
	// y se va a cambiar la estructura con el scene parser

	//TODO: asignar un tipo a cada objeto de la escena
	if (obj->GetName().find("_mesh") != std::string::npos) {
		displayName.erase(displayName.length() - 5);
		displayName[0] = std::toupper(displayName[0]);
		displayName = ICON_FA_CUBE " " + displayName;

	}
	else if (obj->GetName().find("_light") != std::string::npos) {
		displayName.erase(displayName.length() - 6);
		displayName[0] = std::toupper(displayName[0]);
		displayName = ICON_FA_LIGHTBULB " " + displayName;

	}
	else if (obj->GetName().find("_bg") != std::string::npos) {
		displayName.erase(displayName.length() - 3);
		displayName[0] = std::toupper(displayName[0]);
		displayName = ICON_FA_MOUNTAIN_SUN " " + displayName;

	}
	else {
		displayName = ICON_FA_OBJECT_GROUP " " + displayName;
	}
	return displayName;
}

void Outliner::RenderOptions()
{
	static char str1[128] = "";
	ImGui::SameLine();
	ImGui::Button(ICON_FA_ARROW_UP_WIDE_SHORT);

	// Scene object search input
	float remainingWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SameLine();
	ImGui::SetNextItemWidth(remainingWidth * 0.50f);
	ImGui::InputTextWithHint("##SearchItem", ICON_FA_MAGNIFYING_GLASS " Search item", str1, IM_ARRAYSIZE(str1));

	//New Folder
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_CIRCLE_PLUS" New Object")) {
		Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
		auto& resmg = Singleton<Core::Assets::ResourceManager>::Instance();

		std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
		obj->SetPosition(glm::vec3(0.0f,0.0f,0.0f));
		obj->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		obj->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		obj->SetName("New Object");
		obj->SetID("New Object");


		std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> renderer = std::make_shared<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj);
		renderer->SetMesh(resmg.GetResource<::Graphics::Primitives::Model>("Content/Meshes/cube_face.obj"));

		renderer->SetShaderProgram(resmg.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/DeferredGeometry.shader"));

		obj->AddComponent(std::move(renderer));

		scene.addObject(obj);

	}
}

void Outliner::RenderSceneObjects()
{
	Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();

	//Tree with list of selectable items in scene
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	const std::vector<std::shared_ptr<Core::Object>>& sceneObjects = scene.GetObjects();

	if (ImGui::TreeNode(ICON_FA_HOUSE " Main (Editor)")) {
		static std::string selectedNodeChild = "None";

		int i = 0;
		for (const auto& obj : sceneObjects) {
			//Check if object is selected
			boolean isNodeSelected = obj == selectedObj.GetSelectedObject();
			std::string displayName = formatObjName(obj);




			ImGui::SetNextItemAllowOverlap();

			ImGui::PushID(i);

			if (ImGui::Selectable(displayName.c_str(), isNodeSelected)) {
				selectedObj.SetSelectedObject(obj);
				selectedObj.SetSelectedComponent(nullptr);
			}

			ImGui::PopID();
			i++;

		}

		ImGui::TreePop();
	}

	ImGui::Spacing();
	std::string itemNum = std::to_string(sceneObjects.size()) + " items in scene";
	ImGui::Text(itemNum.c_str());

	ImGui::Spacing(); 
}

#include "Outliner.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/ResourceManager.h"
#include "Core/ECSystem/Scene.h"
#include "Core/AppWrapper.h"
#include "Core/ECSystem/Scene.h"
#include "Core/Editor/SelectedObj.h"

SelectedObj& selectedObj = Singleton<SelectedObj>::Instance();


void Outliner::Render(){
	ImGui::Begin("Outliner");

	//Icons 
	ImTextureID filterIcon = nullptr;

	static char str1[128] = "";
	ImGui::SameLine();
	if (ImGui::ImageButton(filterIcon, ImVec2(20, 20))) {
		// filter dropdown
	}

	// Scene object search input
	float remainingWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SameLine();
	ImGui::SetNextItemWidth(remainingWidth * 0.50f);
	ImGui::InputTextWithHint("##SearchItem", "Search item", str1, IM_ARRAYSIZE(str1));


	Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();


	//New Folder
	ImGui::SameLine();
	if (ImGui::Button("New Folder")) {
		for (const auto& obj : scene.GetObjects()) {
			std::cout << "Objeto: " << obj->GetName() << std::endl;
		}
	}



	
	

	//Tree with list of selectable items in scene
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	const std::vector<std::shared_ptr<Core::Object>>& sceneObjects = scene.GetObjects();


	if (ImGui::TreeNode("Scene items")) {
		static std::string selected = "None";
		
		for (const auto& obj : sceneObjects) {

			if (ImGui::TreeNode(obj->GetName().c_str(), obj->GetName().c_str()))
			{

				std::string cMesh = obj->GetName() + "_mesh";
				std::string cLight = obj->GetName() + "_light";

				
				if (ImGui::Selectable("Mesh", selected == cMesh)){
					selected = cMesh;
					selectedObj.SetSelectedObject(obj);
				}

				if (ImGui::Selectable("Light", selected == cLight)) {
					selected = cLight;
					selectedObj.SetSelectedObject(obj);
				}



				ImGui::TreePop();
			}
		}



		ImGui::TreePop();
	}


	ImGui::Spacing();

	std::string itemNum = std::to_string(sceneObjects.size()) + " items in scene";
	ImGui::Text(itemNum.c_str(), IMGUI_VERSION, IMGUI_VERSION_NUM);

	if (selectedObj.GetSelectedObject()) {
		std::string selectedObjInfo = selectedObj.GetSelectedObject()->GetName() + " items in scene";
		ImGui::Text(selectedObjInfo.c_str(), IMGUI_VERSION, IMGUI_VERSION_NUM);
	}


	
	

	ImGui::Spacing();




	ImGui::End();

}

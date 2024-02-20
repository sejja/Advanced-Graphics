#include "Outliner.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/ResourceManager.h"
#include "Core/ECSystem/Scene.h"
#include "Core/AppWrapper.h"
#include "Core/ECSystem/Scene.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"


SelectedObj& selectedObj = Singleton<SelectedObj>::Instance();


void Outliner::Render(){
	ImGui::Begin("Outliner");

	//Icons 
	ImTextureID filterIcon = nullptr;

	static char str1[128] = "";
	ImGui::SameLine();
	ImGui::Button(ICON_FA_ARROW_UP_WIDE_SHORT);

	// Scene object search input
	float remainingWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SameLine();
	ImGui::SetNextItemWidth(remainingWidth * 0.50f);
	ImGui::InputTextWithHint("##SearchItem", ICON_FA_MAGNIFYING_GLASS " Search item", str1, IM_ARRAYSIZE(str1));


	Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();


	//New Folder
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_FOLDER)) {
		for (const auto& obj : scene.GetObjects()) {
			std::cout << "Objeto: " << obj->GetName() << std::endl;
		}
	}



	
	

	//Tree with list of selectable items in scene
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	const std::vector<std::shared_ptr<Core::Object>>& sceneObjects = scene.GetObjects();


	if (ImGui::TreeNode(ICON_FA_HOUSE " Main (Editor)")) {
		static std::string selectedNodeChild = "None";

		
		for (const auto& obj : sceneObjects) {


			ImGui::SetNextItemAllowOverlap(); 


			if (obj->GetName().find("_mesh") != std::string::npos){

				std::string displayName = obj->GetName();
				displayName.erase(displayName.length() - 5);
				displayName[0] = std::toupper(displayName[0]);

				boolean isNodeSelected = obj == selectedObj.GetSelectedObject();


				//OBJECTO MESH
				if (ImGui::TreeNodeEx(obj->GetName().c_str(), isNodeSelected, displayName.c_str())){

					if (ImGui::IsItemClicked()) {
						selectedObj.SetSelectedObject(obj);
					}


					//New LIGHT/MESH COMPONENT 
					ImGui::SameLine();
					ImGui::SmallButton(ICON_FA_SQUARE_PLUS " " ICON_FA_LIGHTBULB);

					ImGui::SameLine();
					ImGui::SmallButton(ICON_FA_SQUARE_PLUS " " ICON_FA_CUBE);


					std::string cMesh = obj->GetName() + "_mesh";
					std::string cLight = obj->GetName() + "_light";


					if (ImGui::Selectable(ICON_FA_CUBE " Mesh", selectedNodeChild == cMesh)) {
						selectedNodeChild = cMesh;
						selectedObj.SetSelectedObject(obj);
					}

					if (ImGui::Selectable(ICON_FA_LIGHTBULB " Light", selectedNodeChild == cLight)) {
						selectedNodeChild = cLight;
						selectedObj.SetSelectedObject(obj);
					}



					ImGui::TreePop();
				}
			}
			else {
				if (ImGui::TreeNode(obj->GetName().c_str(), obj->GetName().c_str())) {
					if (ImGui::IsItemClicked()) {
						selectedObj.SetSelectedObject(obj);
					}
					ImGui::TreePop();
				}
			
			}


		}



		ImGui::TreePop();
	}


	ImGui::Spacing();

	std::string itemNum = std::to_string(sceneObjects.size()) + " items in scene";
	ImGui::Text(itemNum.c_str());

	if (selectedObj.GetSelectedObject()) {
		std::string selectedObjInfo = "Editing: " + selectedObj.GetSelectedObject()->GetName();
		ImGui::Text(selectedObjInfo.c_str());
	}


	
	

	ImGui::Spacing();




	ImGui::End();

}

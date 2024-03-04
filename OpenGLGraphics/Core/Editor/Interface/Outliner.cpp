#include "Outliner.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/ResourceManager.h"
#include "Core/ECSystem/Scene.h"
#include "Core/AppWrapper.h"
#include "Core/ECSystem/Scene.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include "Core/Editor/Editor.h"


SelectedObj& selectedObj = Singleton<Editor>::Instance().GetSelectedObj();


void Outliner::Render(){
	ImGui::Begin(ICON_FA_FOLDER "  Outliner");

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
	if (ImGui::Button(ICON_FA_FOLDER_PLUS)) {
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

			std::string displayName = obj->GetName();

			
			//Check if object is selected
			boolean isNodeSelected = obj == selectedObj.GetSelectedObject();

			// de momento así por que los objetos de la escena no tienen un tipo
			// y probablemente se va a cambiar la estructura pronto
			if (obj->GetName().find("_mesh") != std::string::npos){
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

			if (ImGui::Selectable(displayName.c_str(), isNodeSelected)) {
				selectedObj.SetSelectedObject(obj);
				selectedObj.SetSelectedComponent(nullptr);
			}


		}



		ImGui::TreePop();
	}


	ImGui::Spacing();

	std::string itemNum = std::to_string(sceneObjects.size()) + " items in scene";
	ImGui::Text(itemNum.c_str());




	
	

	ImGui::Spacing();




	ImGui::End();

}

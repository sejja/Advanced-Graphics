#include "Outliner.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/Assets/ResourceManager.h"
#include "Core/AppWrapper.h"
#include "Core/ECSystem/Scene.h"
#include "Core/Editor/SelectedObj.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include "Core/Editor/Editor.h"
#include <random>


SelectedObj& selectedObj = Singleton<Editor>::Instance().GetSelectedObj();


void Outliner::Render(){
	ImGui::Begin(ICON_FA_FOLDER "  Outliner");

	RenderOptions();
	RenderSceneObjects();

	ImGui::End();

}



std::string Outliner::formatObjName(const std::shared_ptr<Core::Object>& obj) {
	std::string displayName = obj->GetName();
	std::string lower = displayName;
	std::transform(lower.begin(), lower.end(), lower.begin(),
		[](unsigned char c) { return std::tolower(c); });
	
	if (lower.find("light") != std::string::npos) {
		displayName = ICON_FA_LIGHTBULB " " + displayName;
	}
	else if (obj->GetName().find("SKYBOX") != std::string::npos) {
		displayName = ICON_FA_MOUNTAIN_SUN " " + displayName;

	}
	else {
		displayName = ICON_FA_CUBE " " + displayName;
	}
	return displayName;
}

void Outliner::RenderOptions()
{
	
	ImGui::SameLine();
	ImGui::Button(ICON_FA_ARROW_UP_WIDE_SHORT);

	// Scene object search input
	float remainingWidth = ImGui::GetContentRegionAvail().x;
	ImGui::SameLine();
	ImGui::SetNextItemWidth(remainingWidth * 0.50f);
	ImGui::InputTextWithHint("##SearchItem", ICON_FA_MAGNIFYING_GLASS " Search item", searchField, IM_ARRAYSIZE(searchField));

	//NEW OBJECT
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_CIRCLE_PLUS" New Object")) {
		Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
		std::shared_ptr<Core::Object> obj = std::make_shared<Core::Object>();
		obj->SetPosition(glm::vec3(0.0f,0.0f,0.0f));
		obj->SetRotation(glm::vec3(0.0f, 0.0f, 0.0f));
		obj->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
		obj->SetName("New Object");
		obj->SetID(generateID());
		scene.addObject(obj);
	}

}

void Outliner::RenderSceneObjects(){
	Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
	//Tree with list of selectable items in scene
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	const std::vector<std::shared_ptr<Core::Object>>& sceneObjects = scene.GetObjects();
	if (ImGui::TreeNode(ICON_FA_HOUSE " Main (Editor)")) {
		static std::string selectedNodeChild = "None";
		int i = 0;
		for (const auto& obj : sceneObjects) {
			if (!obj) { continue; } //Obj could have been deleted in prev frame

			//Particle manager and skybox are not editable
			if (obj->GetID() == "PARTICLE_MANAGER" || obj->GetID() == "SKYBOX") {
				continue;
			}

			std::string lower = obj->GetName();
			std::transform(lower.begin(), lower.end(), lower.begin(),
				[](unsigned char c) { return std::tolower(c); });
			if (searchField != "" && lower.find(searchField) == std::string::npos) {
				continue;
			}



			//Check if object is selected
			boolean isNodeSelected = obj == selectedObj.GetSelectedObject();
			std::string displayName = formatObjName(obj);
			ImGui::SetNextItemAllowOverlap();
			ImGui::PushID(i);

			if (!isNodeSelected || (!editingName && isNodeSelected)) {
				if (ImGui::Selectable(displayName.c_str(), isNodeSelected)) {
					selectedObj.SetSelectedObject(obj);
					selectedObj.SetSelectedComponent(nullptr);
				}
				ImGui::SetItemTooltip("Right-click to open options");
			}
			else {
				static char buffer[256];
				strcpy(buffer, newName.c_str());
				if (ImGui::InputText("", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
					newName = buffer;
					obj->SetName(newName);
					editingName = false;
				}
			}
			
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::MenuItem(ICON_FA_I_CURSOR" Rename")) {
					selectedObj.SetSelectedObject(obj);
					selectedObj.SetSelectedComponent(nullptr);
					newName = obj->GetName();
					editingName = true;
				}


				if (ImGui::MenuItem(ICON_FA_TRASH_CAN" Delete")) {
					Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
					scene.removeObject(obj);
					selectedObj.SetSelectedObject(NULL);
					selectedObj.SetSelectedComponent(NULL);
					selectedObj.SetSelectedObject(NULL);
				}
				ImGui::EndPopup();
			}


			

			ImGui::PopID();
			i++;
		}
		ImGui::TreePop();
	}
	ImGui::Spacing();
	std::string itemNum = std::to_string(sceneObjects.size()-2) + " items in scene";
	ImGui::Text(itemNum.c_str());
	ImGui::Spacing(); 
}

std::string Outliner::generateID() {
	std::random_device rd;
	std::mt19937 gen(rd());
	int min = 10000;
	int max = 99999;
	std::uniform_int_distribution<> dis(min, max);
	int random_number = dis(gen);
	std::string id = std::to_string(random_number);
	return id;
}

			
		
		
	
	


#include "Outliner.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/ResourceManager.h"
#include "Core/ECSystem/Scene.h"
#include "Core/AppWrapper.h"
#include "Core/ECSystem/Scene.h"


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

	//New Folder
	ImGui::SameLine();
	if (ImGui::Button("New Folder")) {

	}


	Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();

	
	for (const auto& obj : scene.GetObjects()) {
		//std::cout << "Objeto: " << obj->GetName() << std::endl;
	}

	//Tree with list of selectable items in scene
	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::TreeNode("Scene items")) {
		for (const auto& obj : scene.GetObjects()) {

			if (ImGui::TreeNode(obj->GetName().c_str(), obj->GetName().c_str()))
			{
				ImGui::Text("asdgasdg");
				ImGui::SameLine();
				if (ImGui::SmallButton("button")) {}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}


	ImGui::Spacing();
	ImGui::Text("4 items in scene", IMGUI_VERSION, IMGUI_VERSION_NUM);
	ImGui::Spacing();




	ImGui::End();

}

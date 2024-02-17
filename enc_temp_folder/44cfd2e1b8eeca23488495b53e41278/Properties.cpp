#include "Properties.h"
#include <iostream>
#include "Dependencies/ImGui/imgui.h"

void Properties::Render() {

	RenderOutliner();
	//RenderProperties();
}

void Properties::RenderOutliner(){
	ImGui::Begin("Outliner");

	//Icons 
	ImTextureID filterIcon = nullptr; 

	static char str1[128] = "";
	ImGui::SameLine();
	if (ImGui::ImageButton(filterIcon, ImVec2(20, 20))) {
		// filter dropdown
	}

	// Scene object search input
	ImGui::SameLine();
	ImGui::InputTextWithHint("##SearchItem", "Search item", str1, IM_ARRAYSIZE(str1));

	//New Folder
	ImGui::SameLine();
	if (ImGui::Button("New Folder")) {
		//new folder action
	}




	//Tree with list of selectable items in scene
	if (ImGui::TreeNode("Scene items")){
		for (int i = 0; i < 5; i++)
		{
			// Use SetNextItemOpen() so set the default state of a node to be open. We could
			// also use TreeNodeEx() with the ImGuiTreeNodeFlags_DefaultOpen flag to achieve the same thing!
			if (i == 0)
				ImGui::SetNextItemOpen(true, ImGuiCond_Once);

			if (ImGui::TreeNode((void*)(intptr_t)i, "Child %d", i))
			{
				ImGui::Text("blah blah");
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



void Properties::RenderProperties(){
	char buf[256];
	float f = 0.0f;

	strcpy(buf, "test");

	ImGui::Text("YEYEYE", 123);
	if (ImGui::Button("Save"))
		printf("TEST");
	ImGui::InputText("string", buf, IM_ARRAYSIZE(buf));
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
}

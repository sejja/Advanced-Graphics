#include "Properties.h"
#include <iostream>
#include "Dependencies/ImGui/imgui.h"

void Properties::Render() {

	RenderOutliner();
	//RenderProperties();
}

void Properties::RenderOutliner(){
	ImGui::Begin("Outiner");

	//Scene object search input 
	char buf[256];
	strcpy(buf, "");
	ImGui::InputText(" ", buf, IM_ARRAYSIZE(buf));






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

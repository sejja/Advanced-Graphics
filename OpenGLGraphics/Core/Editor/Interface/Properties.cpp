#include "Properties.h"
#include <iostream>
#include "Dependencies/ImGui/imgui.h"

void Properties::Render() {
	ImGui::Begin("Properties");

	// Property tool search input
	static char str1[128] = "";

	ImGui::InputTextWithHint("##SearchProperty", "Search property", str1, IM_ARRAYSIZE(str1));
	ImGui::Spacing();

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_None)) {
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		TransformOptions();
	}

	ImGui::End();
}

void DrawColoredTextWithBackground(const char* text, ImVec4 bgColor) {
    ImVec2 labelSize = ImGui::CalcTextSize(text);
    ImVec2 startPos = ImGui::GetCursorScreenPos();
    ImVec2 endPos = ImVec2(startPos.x + 2.5f + labelSize.x + 2.5f, startPos.y + labelSize.y + 6.0f);

    ImGui::GetWindowDrawList()->AddRectFilled(startPos, endPos, IM_COL32((int)(bgColor.x * 255), (int)(bgColor.y * 255), (int)(bgColor.z * 255), (int)(bgColor.w * 255))); // Fondo
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), text);
}


void TransformRow(const char* title) {
    static char defaultValue[16] = "0";

    ImGui::Text(title); ImGui::SameLine();

    if (ImGui::Button("Lock")) {
    }

    float remainingWidth = ImGui::GetContentRegionAvail().x;

    static char buf1[32] = "0.0";
    std::string XLabel = "X";
    std::string XID = "##X" + std::string(title);
    ImGui::SameLine();
    DrawColoredTextWithBackground(XLabel.c_str(), ImVec4(1.0f, 0.5f, 0.5f, 0.7f));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(remainingWidth * 0.10f);
    ImGui::InputText(XID.c_str(), buf1, IM_ARRAYSIZE(buf1), ImGuiInputTextFlags_CharsDecimal);

    static char buf2[32] = "0.0";
    std::string YLabel = "Y";
    std::string YID = "##X" + std::string(title);
    ImGui::SameLine();
    DrawColoredTextWithBackground(YLabel.c_str(), ImVec4(0.5f, 1.0f, 0.5f, 0.7f));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(remainingWidth * 0.10f);
    ImGui::InputText(YID.c_str(), buf2, IM_ARRAYSIZE(buf2), ImGuiInputTextFlags_CharsDecimal);

    static char buf3[32] = "0.0";
    std::string ZLabel = "Z";
    std::string ZID = "##X" + std::string(title);
    ImGui::SameLine();
    DrawColoredTextWithBackground(ZLabel.c_str(), ImVec4(0.5f, 0.5f, 1.0f, 0.7f));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(remainingWidth * 0.10f);
    ImGui::InputText(ZID.c_str(), buf3, IM_ARRAYSIZE(buf3), ImGuiInputTextFlags_CharsDecimal);

    ImGui::SameLine();
    ImGui::Button("Reset");

}




void Properties::TransformOptions(){
    TransformRow("Location");
    TransformRow("Rotation");
    TransformRow(" Scale  ");

    
}









#include "Properties.h"
#include <iostream>
#include <format>
#include "Dependencies/ImGui/imgui.h"
#include "Core/ResourceManager.h"
#include "Graphics/Primitives/Texture.h"
#include "Core/Singleton.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Dependencies/ImGui/imgui_impl_opengl3.h"
#include <Dependencies/ImGui/imgui_impl_sdl2.h>
#include "Core/Editor/SelectedObj.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include "Core/Editor/Interface/AssetIcon.h"


SelectedObj& selectedObjIns = Singleton<SelectedObj>::Instance();



void Properties::Render() {
	ImGui::Begin(ICON_FA_SLIDERS " Properties");

	// Property tool search input
	static char str1[128] = "";

    objectOutlinerComp();


	ImGui::InputTextWithHint("##SearchProperty", ICON_FA_MAGNIFYING_GLASS " Search property", str1, IM_ARRAYSIZE(str1));
	ImGui::Spacing();




    if (selectedObjIns.GetSelectedObject()) {
        if (ImGui::CollapsingHeader(ICON_FA_ARROWS_TO_DOT " Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            TransformOptions();
        }
    }

    if (ImGui::CollapsingHeader(ICON_FA_LIGHTBULB "  Light", ImGuiTreeNodeFlags_DefaultOpen)) {
        LightingOptions();
    }

    if (ImGui::CollapsingHeader(ICON_FA_BRUSH "  Material", ImGuiTreeNodeFlags_DefaultOpen)) {
		MaterialsOptions();
	}

    if (ImGui::CollapsingHeader(ICON_FA_SHAPES "  Static Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
        MaterialsOptions();
    }



	ImGui::End();
}

void TextPaddingWBg(const char* text, ImVec4 bgColor) {
    ImVec2 labelSize = ImGui::CalcTextSize(text);
    ImVec2 startPos = ImGui::GetCursorScreenPos();
    ImVec2 endPos = ImVec2(startPos.x + labelSize.x + 4.5f, startPos.y + labelSize.y + 6.0f);
    float borderRadius = 3.0f;

    startPos.x -= 4.5f;

    ImGui::GetWindowDrawList()->AddRectFilled(startPos, endPos, IM_COL32((int)(bgColor.x * 255), (int)(bgColor.y * 255), (int)(bgColor.z * 255), (int)(bgColor.w * 255)), borderRadius); // Fondo
    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), text);
}


void TransformRow(const char* title,float& x_val, float& y_val, float& z_val) {
    //TODO CONVERTIRLO EN UNA GRID
    static char defaultValue[16] = "0";
    const float f32_zero = -1000.f;
    const float f32_one = 1000.f;
    const float dragJump = 0.5f;
    const float inputSize = 0.12f;
    float remainingWidth = ImGui::GetContentRegionAvail().x;

    ImGui::SetNextItemWidth(remainingWidth * inputSize);

    ImGui::PushItemWidth(100);//No funciona
    ImGui::Text(title); 
    ImGui::PopItemWidth();
    ImGui::SameLine();


    ImGui::Button(ICON_FA_LOCK);


    std::string XLabel = "X";
    char XID[32];
    sprintf(XID, "##%d%d", XLabel, title);
    ImGui::SameLine();
    TextPaddingWBg(XLabel.c_str(), ImVec4(1.0f, 0.5f, 0.5f, 0.7f));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(remainingWidth * inputSize);
    ImGui::DragScalar(XID, ImGuiDataType_Float, &x_val, dragJump, &f32_zero, &f32_one, "%.2f");


    std::string YLabel = "Y";
    std::string YID = "##Y" + std::string(title);
    ImGui::SameLine();
    TextPaddingWBg(YLabel.c_str(), ImVec4(0.5f, 1.0f, 0.5f, 0.7f));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(remainingWidth * inputSize);
    ImGui::DragScalar(YID.c_str(), ImGuiDataType_Float, &y_val, dragJump, &f32_zero, &f32_one, "%.2f");

    std::string ZLabel = "Z";
    std::string ZID = "##Z" + std::string(title);
    ImGui::SameLine();
    TextPaddingWBg(ZLabel.c_str(), ImVec4(0.5f, 0.5f, 1.0f, 0.7f));
    ImGui::SameLine();
    ImGui::SetNextItemWidth(remainingWidth * inputSize);
    ImGui::DragScalar(ZID.c_str(), ImGuiDataType_Float, &z_val, dragJump, &f32_zero, &f32_one, "%.2f");



    auto IconTex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("../Assets/Icons/reload.png")->Get();
    ImGui::SameLine();

    if (ImGui::Button(ICON_FA_ARROW_ROTATE_LEFT)) {
        x_val = 0.0f;
        y_val = 0.0f;
        z_val = 0.0f;
    };



}


void Properties::objectOutlinerComp() {
    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();

    //obj->GetComponent<Core::Graphics::Model>();

    if (obj) {
        float originalTextSize = ImGui::GetFontSize();
       
        ImGui::Text("%s %s", ICON_FA_CUBES, obj->GetName().c_str());
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_PLUS " Add")) {
            ImGui::OpenPopup("new_component_modal");
        }

        if (ImGui::BeginPopup("new_component_modal"))
        {
            ImGui::SeparatorText("Components");

            if (ImGui::Selectable(ICON_FA_CUBE " Mesh")) {
				//obj->AddComponent<Core::Graphics::Mesh>();  
            }
            if (ImGui::Selectable(ICON_FA_LIGHTBULB " Light")) {
                //obj->AddComponent<Core::Graphics::Light>();
            }
            
            ImGui::EndPopup();
        }



        selectedObjectTree();
        ImGui::Spacing();
    }

}




void Properties::selectedObjectTree() {
    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();

    ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 2), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);


        std::string displayName = std::format("{} {} (Instance)", ICON_FA_CUBE, obj->GetName());


        printf(displayName.c_str());

        //el selcted seria obj== selobj and comp 
        if (ImGui::Selectable(displayName.c_str(), true)) {
            selectedObjIns.SetSelectedObject(obj);
        }

        ImGui::Indent();


        //OBJECT COMPONENTS

        //obj->GetComponent<Mesh>()


        if (ImGui::Selectable(ICON_FA_CUBE " Mesh",false)) {
            selectedObjIns.SetSelectedObject(obj);
        }

        if (ImGui::Selectable(ICON_FA_LIGHTBULB " Light", false)) {
            selectedObjIns.SetSelectedObject(obj);
        }
        ImGui::Unindent();


    ImGui::EndChildFrame();




    
    
    


}






void Properties::TransformOptions() {
    
    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();

    
    glm::vec3 curPos = obj->GetPosition();
    glm::vec3 curRot = obj->GetRotation();
    glm::vec3 curScale = obj->GetScale();
    
    TransformRow("Location", curPos[0], curPos[1], curPos[2]);
    TransformRow("Rotation", curRot[0], curRot[1], curRot[2]);
    TransformRow("  Scale    ", curScale[0], curScale[1], curScale[2]);

    obj->SetPosition(curPos);
    obj->SetRotation(curRot);
    obj->SetScale(curScale);


    
}



static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

void colorPickerBtn() {

    ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_AlphaPreviewHalf;
    static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
    static ImVec4 backup_color;

    // Generate a default palette. The palette will persist and can be edited.
    static bool saved_palette_init = true;
    static ImVec4 saved_palette[32] = {};
    if (saved_palette_init)
    {
        for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
        {
            ImGui::ColorConvertHSVtoRGB(n / 31.0f, 0.8f, 0.8f,
                saved_palette[n].x, saved_palette[n].y, saved_palette[n].z);
            saved_palette[n].w = 1.0f; // Alpha
        }
        saved_palette_init = false;
    }


    if (ImGui::ColorButton("Light Color ##3c", *(ImVec4*)&color, misc_flags, ImVec2(80, 40))) {
        ImGui::OpenPopup("colorPicker");
        backup_color = color;
    }



    if (ImGui::BeginPopup("colorPicker")) {
        ImGui::Separator();
        ImGui::ColorPicker4("##picker", (float*)&color, misc_flags | ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoSmallPreview);
        ImGui::SameLine();

        ImGui::BeginGroup(); // Lock X position
        ImGui::Text("Current");
        ImGui::ColorButton("##current", color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40));
        ImGui::Text("Previous");
        if (ImGui::ColorButton("##previous", backup_color, ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_AlphaPreviewHalf, ImVec2(60, 40)))
            color = backup_color;
        ImGui::Separator();
        ImGui::Text("Palette");
        for (int n = 0; n < IM_ARRAYSIZE(saved_palette); n++)
        {
            ImGui::PushID(n);
            if ((n % 8) != 0)
                ImGui::SameLine(0.0f, ImGui::GetStyle().ItemSpacing.y);

            ImGuiColorEditFlags palette_button_flags = ImGuiColorEditFlags_NoAlpha | ImGuiColorEditFlags_NoPicker | ImGuiColorEditFlags_NoTooltip;
            if (ImGui::ColorButton("##palette", saved_palette[n], palette_button_flags, ImVec2(20, 20)))
                color = ImVec4(saved_palette[n].x, saved_palette[n].y, saved_palette[n].z, color.w); // Preserve alpha!

            // Allow user to drop colors into each palette entry. Note that ColorButton() is already a
            // drag source by default, unless specifying the ImGuiColorEditFlags_NoDragDrop flag.
            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_3F))
                    memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 3);
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(IMGUI_PAYLOAD_TYPE_COLOR_4F))
                    memcpy((float*)&saved_palette[n], payload->Data, sizeof(float) * 4);
                ImGui::EndDragDropTarget();
            }

            ImGui::PopID();
        }
        ImGui::EndGroup();
        ImGui::EndPopup();
    }

}

void CreateSliderRow(const char* rowName , float& sliderValue , float minValue, float maxValue ) {
    ImGui::TableSetColumnIndex(0);
    ImGui::Text(rowName);
    ImGui::TableSetColumnIndex(1);

    static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    std::string SLIDERID = "##" + std::string(rowName);
    ImGui::SliderFloat(SLIDERID.c_str(), &sliderValue, minValue, maxValue, "%.3f");
    ImGui::SameLine();

    auto reloadText = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Core/Editor/Assets/Icons/lockClosed.jpg")->Get();
    ImGui::ImageButton((void*)(intptr_t)reloadText->GetTextureHandle(), ImVec2(13, 13));


}


void Properties::LightingOptions() {
    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);


    if (ImGui::BeginTable("light_table", 2, flags1)) {

        
        ImGui::TableNextRow();
        static float intensity = 0.5f;
        CreateSliderRow("Intensity", intensity,0.0f,100.0f);


        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Light Color");
        ImGui::TableSetColumnIndex(1);
        colorPickerBtn(); //esta todo dentro por que solo el hue esta dentro no devuelve bien el clr

        ImGui::TableNextRow();
        static float scrrange = 0.5f;
        CreateSliderRow("Source Range", scrrange, 0.0f, 100.0f);

        ImGui::TableNextRow();
        static float scrlength = 0.5f;
        CreateSliderRow("Source Length", scrlength, 0.0f, 100.0f);

        



        ImGui::EndTable();
    }

    ImGui::PopStyleVar();





}
//TODO componetizar tabla y usar en todas los componentes

void Properties::MaterialsOptions(){
    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);


    ImGuiDragDropFlags flags = 0;
    flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
    std::string texto = "No se ha droppeado nada";
    if (ImGui::BeginDragDropTarget()) {
        printf("Dropping");
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("other", flags)) {
            AssetIcon* iconPtr = (AssetIcon*)payload->Data;
            texto = iconPtr->nombre;
        }
        ImGui::EndDragDropTarget();
    }

    if (ImGui::BeginTable("materials_table", 2, flags1)) {

        ImGui::TableNextRow();
       
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Element 0");
        ImGui::TableSetColumnIndex(1);

       
        auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content\\Textures\\Brick.png")->Get();
        ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));


        ImGui::SameLine();
        ImGui::BeginGroup();

        ImGui::Text(texto.c_str());

        ImGui::Button(ICON_FA_ARROW_TURN_UP);
        ImGui::SameLine();
        ImGui::Button(ICON_FA_FOLDER_OPEN);


        ImGui::EndGroup();


    }
    ImGui::EndTable();

    ImGui::PopStyleVar();


}

void Properties::MeshOptions(){
    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

    if (ImGui::BeginTable("mesh_table", 2, flags1)) {

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Element 0");
        ImGui::TableSetColumnIndex(1);


        auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content\\Textures\\Brick.png")->Get();
        ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));


        ImGui::SameLine();
        ImGui::BeginGroup();

        if (ImGui::BeginCombo("", "Nombre textura")) {
            ImGui::Selectable("Textura 1");
            ImGui::Selectable("Textura 2");
            ImGui::Selectable("Textura 3");
            ImGui::EndCombo();
        }

        ImGui::Button(ICON_FA_ARROW_TURN_UP);
        ImGui::SameLine();
        ImGui::Button(ICON_FA_FOLDER_OPEN);


        ImGui::EndGroup();


    }
    ImGui::EndTable();

    ImGui::PopStyleVar();

}










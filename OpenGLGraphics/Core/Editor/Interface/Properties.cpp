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
#include <Graphics/Primitives/Model.h>
#include <Graphics/Primitives/Light.h>
#include <Core/ECSystem/Component.h>
#include <Graphics/Primitives/Skybox.h>
#include "Core/Editor/Editor.h"
#include "Core/Network/Server.h"
#include "Core/Network/Client.h"
#include "Core/ParticleSystem/ParticleSystem.h"
#include "Core/ParticleSystem/FireSystem.h"


//SelectedObj& selectedObjIns = Singleton<SelectedObj>::Instance();

SelectedObj& selectedObjIns = Singleton<Editor>::Instance().GetSelectedObj();


//SACAR LOS SENDTOPEER DE AQUI 
void sendToPeer(std::shared_ptr<Core::Object> obj) {
    Server& server = Singleton<Server>::Instance();
    Client& client = Singleton<Client>::Instance();

    if (server.isRunning()) {
        server.sendObjectIfChanged(obj);
    }
    else if (client.isConnected()) {
        client.sendObjectIfChanged(obj);
    }

}

void sendToPeer(std::shared_ptr<Core::Particles::FireSystem> particleSys) {
    Server& server = Singleton<Server>::Instance();
    Client& client = Singleton<Client>::Instance();


    if (server.isRunning()) {
        server.sendParticleIfChanged(particleSys);
    }
    else if (client.isConnected()) {
        client.sendParticleIfChanged(particleSys);
    }
}


void Properties::Render() {
	ImGui::Begin(ICON_FA_SLIDERS " Properties");

    std::shared_ptr<Core::Particles::ParticleSystem> particleSystem = std::dynamic_pointer_cast<Core::Particles::ParticleSystem>(selectedObjIns.GetSelectedComponent());
    std::shared_ptr<::Graphics::Primitives::Light> lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Light>(selectedObjIns.GetSelectedComponent());
    std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>> meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>>(selectedObjIns.GetSelectedComponent());
    std::shared_ptr<Core::Particles::FireSystem> fireSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(selectedObjIns.GetSelectedComponent());

	// Property tool search input
	static char search_input_field[128] = "";
	ImGui::InputTextWithHint("##SearchProperty", ICON_FA_MAGNIFYING_GLASS " Search property", search_input_field, IM_ARRAYSIZE(search_input_field));
    ImGui::Spacing();

    if (selectedObjIns.GetSelectedObject() && !particleSystem) {
        //La lista de componentes del objeto seleccionado
        objectOutliner();
    }
    
    if (meshComp) {

        if (ImGui::CollapsingHeader(ICON_FA_ARROWS_TO_DOT " Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            TransformOptions();
        }

        if (ImGui::CollapsingHeader(ICON_FA_BRUSH "  Material", ImGuiTreeNodeFlags_DefaultOpen)) {
            MaterialsOptions();
        }

        if (ImGui::CollapsingHeader(ICON_FA_SHAPES "  Static Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
            MeshOptions();
        }
    }
    else if (lightComp) {
        if (ImGui::CollapsingHeader(ICON_FA_LIGHTBULB "  Light", ImGuiTreeNodeFlags_DefaultOpen)) {
            LightingOptions();
        }
    }

    else if (fireSystem) {

        if (ImGui::CollapsingHeader(ICON_FA_ARROWS_TO_DOT " Particle Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ParticleTransform();
            FireSize();
            sendToPeer(fireSystem);
        }

	}

    else {
		ImGui::Text("No component selected");
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
    static char defaultValue[16] = "0";
    const float f32_zero = -1000.f;
    const float f32_one = 1000.f;
    const float dragJump = 0.5f;
    const float inputSize = 0.12f;
    float remainingWidth = ImGui::GetContentRegionAvail().x;


   
    if (ImGui::BeginTable(title, 2)) {
        ImGui::TableSetupColumn("Columna 1", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Columna 2");


        ImGui::TableNextRow();

		ImGui::TableSetColumnIndex(0);
        
		ImGui::Text(title);
		ImGui::TableSetColumnIndex(1);

		ImGui::Button(ICON_FA_LOCK);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(remainingWidth * inputSize);
		ImGui::DragScalar("##X", ImGuiDataType_Float, &x_val, dragJump, &f32_zero, &f32_one, "%.2f");
		ImGui::SameLine();
		TextPaddingWBg("X", ImVec4(1.0f, 0.5f, 0.5f, 0.7f));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(remainingWidth * inputSize);
		ImGui::DragScalar("##Y", ImGuiDataType_Float, &y_val, dragJump, &f32_zero, &f32_one, "%.2f");
		ImGui::SameLine();
		TextPaddingWBg("Y", ImVec4(0.5f, 1.0f, 0.5f, 0.7f));
		ImGui::SameLine();
		ImGui::SetNextItemWidth(remainingWidth * inputSize);
		ImGui::DragScalar("##Z", ImGuiDataType_Float, &z_val, dragJump, &f32_zero, &f32_one, "%.2f");
		ImGui::SameLine();
		TextPaddingWBg("Z", ImVec4(0.5f, 0.5f, 1.0f, 0.7f));

        ImGui::SameLine();

        if (ImGui::Button(ICON_FA_ARROW_ROTATE_LEFT)) {
            x_val = 0.0f;
            y_val = 0.0f;
            z_val = 0.0f;
        };


		ImGui::EndTable();
    
    }

}



void Properties::objectOutliner() {
    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();

    if (obj) {
        ImGui::Text("%s %s", ICON_FA_CUBES, obj->GetName().c_str());ImGui::SameLine();

        if (ImGui::Button(ICON_FA_PLUS " Add")) {
            ImGui::OpenPopup("new_component_modal");
        }
        if (ImGui::BeginPopup("new_component_modal")){

            ImGui::SeparatorText("Components");

            if (ImGui::Selectable(ICON_FA_CUBE " Mesh")) {
                auto renderer = std::make_shared<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>>(obj);
                obj->AddComponent(std::move(renderer));
            }
            if (ImGui::Selectable(ICON_FA_LIGHTBULB " Light")) {
				auto light = std::make_shared<Graphics::Primitives::Light>(obj);
				obj->AddComponent(std::move(light));
            }
            
            ImGui::EndPopup();
        }
        selectedObjectTree();ImGui::Spacing();
    }
}




void Properties::selectedObjectTree() {
    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();

    std::vector<std::shared_ptr<Core::Component>> comps = obj->GetAllComponents();

    //printf("Componentes: %d\n", comps.size());

    ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 2), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);

        std::string displayName = std::format("{} {} (Instance)", ICON_FA_CUBE, obj->GetName());


        bool isRootSelectedObj = selectedObjIns.GetSelectedObject() == obj && (selectedObjIns.GetSelectedComponent() == NULL);

        if (ImGui::Selectable(displayName.c_str(), isRootSelectedObj)) {
            selectedObjIns.SetSelectedObject(obj);
        }

        ImGui::Indent();

        for (auto& comp : comps) {
            bool isCompSelectedObj = selectedObjIns.GetSelectedComponent() == comp;

            // Check if the component is a mesh
            if (std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>> modelComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>>(comp)) {
                if (ImGui::Selectable(ICON_FA_CUBE " Mesh", isCompSelectedObj)) {
                    selectedObjIns.SetSelectedComponent(comp);
                }
           }
            // Check if the component is a light
            else if (std::shared_ptr<::Graphics::Primitives::Light> lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Light>(comp)) {
                if (ImGui::Selectable(ICON_FA_LIGHTBULB " Light", isCompSelectedObj)) {
					selectedObjIns.SetSelectedComponent(comp);
				}
			}
            else {
                if (ImGui::Selectable(ICON_FA_FIRE" Mubi", isCompSelectedObj)) {
                    selectedObjIns.SetSelectedComponent(comp);
                }
            }
			
        }

        ImGui::Unindent();

    ImGui::EndChildFrame();
}




void Properties::ParticleTransform() {

    std::shared_ptr<Core::Component> particleComp = selectedObjIns.GetSelectedComponent();
    std::shared_ptr<Core::Particles::FireSystem> particleSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(particleComp);

    glm::vec3 curCenter = particleSystem->GetSystemCenter();

    TransformRow("Center", curCenter[0], curCenter[1], curCenter[2]);

    particleSystem->SetSystemCenter(curCenter);

    




}




void Properties::TransformOptions() {
    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();

    glm::vec3 curPos = obj->GetPosition();
    glm::vec3 curRot = obj->GetRotation();
    glm::vec3 curScale = obj->GetScale();
    
    TransformRow("  Location", curPos[0], curPos[1], curPos[2]);
    TransformRow("  Rotation", curRot[0], curRot[1], curRot[2]);
    TransformRow("  Scale", curScale[0], curScale[1], curScale[2]);

    obj->SetPosition(curPos);
    obj->SetRotation(curRot);
    obj->SetScale(curScale);

    sendToPeer(obj);

}




void colorPickerBtn(static ImVec4 &color) {

    ImGuiColorEditFlags misc_flags = ImGuiColorEditFlags_HDR | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoBorder | ImGuiColorEditFlags_AlphaPreviewHalf;
    
    static ImVec4 backup_color;

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

    std::shared_ptr<::Graphics::Primitives::Light> lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Light>(selectedObjIns.GetSelectedComponent());

    glm::vec3 attenuation = lightComp->GetAttenuation(); // Constant , Linear , Quadratic

    glm::vec3 specular = lightComp->GetSpecular();
    ImVec4 color = ImVec4(specular.x, specular.y, specular.z, 1.0f);

    if (ImGui::BeginTable("light_table", 2, flags1)) {

        //Light TYPE
        const char* lightTypes[] = { "SPOT", "BBBB", "CCCC" };
        static int item_current_idx = 0;
        const char* initVal = lightTypes[item_current_idx];
        
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Light type");
        ImGui::TableSetColumnIndex(1);


        if (ImGui::BeginCombo("", initVal)) {

            for (int n = 0; n < IM_ARRAYSIZE(lightTypes); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(lightTypes[n], is_selected))
                    item_current_idx = n;

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        
        ImGui::TableNextRow();
        CreateSliderRow("Intensity", attenuation[0], 0.0f, 1.0f);

        //COLOR DE LA LUZ
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Light Color");
        ImGui::TableSetColumnIndex(1);
        colorPickerBtn(color);

        ImGui::TableNextRow(); 
        CreateSliderRow("Source Range", attenuation[1], 0.0f, 1.0f);// Atennuation Quadratic

        ImGui::TableNextRow();
        CreateSliderRow("Source Length", attenuation[2], 0.0f, 1.0f);// Attenuation Linear 

        ImGui::EndTable();
    }

    lightComp->SetAttenuation(attenuation);

    const glm::vec3& colorVec3 = *reinterpret_cast<const glm::vec3*>(&color);
    lightComp->SetSpecular(colorVec3);

    ImGui::PopStyleVar();





}
//TODO componetizar tabla y usar en todas los componentes

static std::string nombreTexturaTemporal = "Textura def";

void Properties::MaterialsOptions(){

    std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>> meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::GraphicsAPIS::OpenGL>>(selectedObjIns.GetSelectedComponent());

    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);


    ImGuiDragDropFlags flags = 0;
    flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;

    
  
    if (ImGui::BeginTable("materials_table", 2, flags1)) {

        ImGui::TableNextRow();
       
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Element 0");
        ImGui::TableSetColumnIndex(1);

       
        auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content\\Textures\\Brick.png")->Get();
        ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));


        ImGui::SameLine();
        ImGui::BeginGroup();


        ImGui::Text(nombreTexturaTemporal.c_str());

        ImGui::Button(ICON_FA_ARROW_TURN_UP);
        ImGui::SameLine();
        ImGui::Button(ICON_FA_FOLDER_OPEN);


        ImGui::EndGroup();


    }
    ImGui::EndTable();
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("other", flags)) {
            AssetIcon* iconPtr = (AssetIcon*)payload->Data;
            nombreTexturaTemporal = iconPtr->nombre;
            printf("RUTA: %s\n", iconPtr->ruta);
            auto nuevaTex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>(iconPtr->ruta)->Get();

            
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopStyleVar();


}

void Properties::MeshOptions(){}

void Properties::FireSize(){   
    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

    
    std::shared_ptr<Core::Particles::FireSystem> fireSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(selectedObjIns.GetSelectedComponent());

    glm::vec3 curRadius = fireSystem->GetRadiusVector();
    float curGap = fireSystem->GetFireGap();
    float curHeight = fireSystem->getHeigth();
    float curParticleSize = fireSystem->GetParticleSize();
    glm::vec4 color = fireSystem->GetBaseColor();
    ImVec4 baseColor = ImVec4(color.x, color.y, color.z, color.w);



    TransformRow("  Radius    ", curRadius[0], curRadius[1], curRadius[2]);


    if (ImGui::BeginTable("fire_size_table", 2, flags1)) {

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Height");
        ImGui::TableSetColumnIndex(1);
        ImGui::SliderFloat("##Height", &curHeight, 0.0f, 100.0f, "%.3f");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Gap");
        ImGui::TableSetColumnIndex(1);
        ImGui::SliderFloat("##Gap", &curGap, 0.0f, 1.0f, "%.3f");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Particle Size");
        ImGui::TableSetColumnIndex(1);
        ImGui::SliderFloat("##ParticleSize", &curParticleSize, 1.0f, 15.0f, "%.3f");

        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Light Color");
        ImGui::TableSetColumnIndex(1);
        colorPickerBtn(baseColor);


        ImGui::EndTable();
    }

    ImGui::PopStyleVar();

    fireSystem->ChangeFireSize(curRadius[0], curRadius[1], curRadius[2], curGap, curHeight);
    fireSystem->SetParticleSize(curParticleSize);

    glm::vec4 newColor = glm::vec4(baseColor.x, baseColor.y, baseColor.z, baseColor.w);
    fireSystem->SetBaseColor(newColor);


}









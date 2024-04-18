#include "Properties.h"
#include <iostream>
#include <format>

#include "Dependencies/ImGui/imgui.h"
#include "Dependencies/ImGui/imgui_internal.h"
#include "Dependencies/ImGui/imgui_impl_opengl3.h"
#include <Dependencies/ImGui/imgui_impl_sdl2.h>

#include "Core/Editor/SelectedObj.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include "Core/Editor/Interface/AssetIcon.h"
#include "Core/Assets/ResourceManager.h"
#include "Core/Singleton.h"
#include "Core/Editor/Editor.h"
#include "Core/Network/Server.h"
#include "Core/Network/Client.h"
#include "Core/ParticleSystem/ParticleSystem.h"
#include "Core/ParticleSystem/FireSystem.h"

#include "Graphics/Primitives/GLBModel.h"
#include "Graphics/Primitives/Renderables.h"
#include "Graphics/Primitives/ShaderProgram.h"
#include "Graphics/Primitives/Texture.h"
#include "Graphics/Primitives/Lights/DirectionalLight.h"
#include "Graphics/Primitives/Lights/PointLight.h"
#include "Graphics/Primitives/Lights/SpotLight.h"
#include <Core/ECSystem/Scene.h>
#include <Core/AppWrapper.h>
#include "Graphics/Primitives/Decal.h"
#include "Core/RTTI/RTTI.h"


SelectedObj& selectedObjIns = Singleton<Editor>::Instance().GetSelectedObj();
auto& resmg = Singleton<Core::Assets::ResourceManager>::Instance();


//TODO: SACAR LOS SENDTOPEER DE AQUI 
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


void Properties::Render(Core::Graphics::OpenGLPipeline& pipeline) {
    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();
    std::shared_ptr<Core::Component> comp = selectedObjIns.GetSelectedComponent();

    std::shared_ptr<Core::Particles::ParticleSystem> particleSystem = NULL;
    std::shared_ptr<::Graphics::Primitives::Lights::Light> lightComp = NULL;
    std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> meshComp = NULL;
    std::shared_ptr<Core::Particles::FireSystem> fireSystem = NULL;
    std::shared_ptr<Graphics::Primitives::Decal> decal = NULL;

    particleSystem = std::dynamic_pointer_cast<Core::Particles::ParticleSystem>(comp);


    //bool isParticleManager = obj->GetID().c_str() == "PARTICLE_MANAGER";
    

    ImGui::Begin(ICON_FA_SLIDERS " Properties");

    if (obj && !particleSystem) {
        objectOutliner(); //La lista de componentes del objeto seleccionado
    }

    //El comp puede haber cambiado en el objectOutliner
    comp = selectedObjIns.GetSelectedComponent();
    particleSystem = std::dynamic_pointer_cast<Core::Particles::ParticleSystem>(comp);
    lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::Light>(comp);
    meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp);
    fireSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(comp);
    decal = std::dynamic_pointer_cast<Graphics::Primitives::Decal>(comp);


    
    if (obj && !comp ) { // && !isParticleManager
        if (ImGui::CollapsingHeader(ICON_FA_ARROWS_TO_DOT " Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            TransformOptions();
            UpdateLightCompsPos(obj);
            sendToPeer(obj);
        }
	}

    if (meshComp) {
        if (ImGui::CollapsingHeader(ICON_FA_BRUSH "  Material", ImGuiTreeNodeFlags_DefaultOpen)) {
            MaterialsOptions();
        }
        if (ImGui::CollapsingHeader(ICON_FA_SHAPES "  Static Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
            MeshOptions();
        }
        if (ImGui::CollapsingHeader(ICON_FA_SUN "  Shader Program", ImGuiTreeNodeFlags_DefaultOpen)) {
            ShaderOptions(pipeline);
        }
    }

    else if (lightComp) {

        if (ImGui::CollapsingHeader(ICON_FA_ARROWS_TO_DOT " Light Position", ImGuiTreeNodeFlags_DefaultOpen)) {
            LightTransform();
        }
        if (ImGui::CollapsingHeader(ICON_FA_LIGHTBULB "  Light Options", ImGuiTreeNodeFlags_DefaultOpen)) {
            LightingOptions();
        }

        if (ImGui::CollapsingHeader(ICON_FA_LIGHTBULB "Light Type Options", ImGuiTreeNodeFlags_DefaultOpen)) {
            LightTypeOptions();
        }

    }

    else if (fireSystem) {
        if (ImGui::CollapsingHeader(ICON_FA_ARROWS_TO_DOT " Particle Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ParticleTransform();
            FireSize();
            sendToPeer(fireSystem);
        }
    }

    else if (decal) {
        if (ImGui::CollapsingHeader(ICON_FA_IMAGE " Textures", ImGuiTreeNodeFlags_DefaultOpen)) {
            DecalOptions();
        }
    }

    else {
        ImGui::Spacing();
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


void TransformRow(const char* title, float& x_val, float& y_val, float& z_val) {
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

    //TODO : Object y no particle manager
    if (obj) {
        ImGui::Text("%s %s", ICON_FA_CUBES, obj->GetName().c_str()); ImGui::SameLine();

        if (ImGui::Button(ICON_FA_PLUS " Add")) {
            ImGui::OpenPopup("new_component_modal");
        }ImGui::SameLine();

        if (ImGui::Button(ICON_FA_TRASH " Delete")) {
            ImGui::OpenPopup("delete_object_modal");
        }

        if (ImGui::BeginPopup("new_component_modal")) {

            ImGui::SeparatorText("Components");

            if (ImGui::Selectable(ICON_FA_CUBE " Mesh")) {
                auto renderer = std::make_shared<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(obj);
                obj->AddComponent(std::move(renderer));
            }

            if (ImGui::Selectable(ICON_FA_LIGHTBULB " Light")) {
                std::shared_ptr<::Graphics::Primitives::PointLight> light;
                light = std::move(std::make_shared<::Graphics::Primitives::PointLight>(obj));
                light->SetRadius(1.0f);
                light->SetFallOff(0.5f);

                light->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));   
                glm::vec3 relativePos = glm::vec3(0.0f, 0.0f, 0.0f);
                light->SetPosition(relativePos);

                obj->AddComponent(std::move(light));

            }

            if (ImGui::Selectable(ICON_FA_NOTE_STICKY " Decal")) {
                obj->AddComponent(std::move(std::make_shared<Graphics::Primitives::Decal>(obj)));
            }

            ImGui::EndPopup();
        }

        if (ImGui::BeginPopup("delete_object_modal")) {

            ImGui::SeparatorText("Confirm Delete Object");

            if (ImGui::Selectable(ICON_FA_TRASH_ARROW_UP " Delete")) {
                Core::Scene& scene = Singleton<AppWrapper>::Instance().getScene();
                scene.removeObject(obj);
            }



            ImGui::EndPopup();
        }

        selectedObjectTree(); ImGui::Spacing();
    }
}




void Properties::selectedObjectTree() {

    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();
    std::vector<std::shared_ptr<Core::Component>> comps = obj->GetAllComponents();

    ImGui::BeginChild("ResizableChild", ImVec2(-FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 2), ImGuiChildFlags_Border | ImGuiChildFlags_ResizeY);

    std::string displayName = std::format("{} {} (Instance)", ICON_FA_CUBE, obj->GetName());

    bool isRootSelectedObj = (selectedObjIns.GetSelectedComponent() == NULL);

    if (ImGui::Selectable(displayName.c_str(), isRootSelectedObj)) {
        selectedObjIns.SetSelectedObject(obj);
        selectedObjIns.SetSelectedComponent(NULL);
    }

    ImGui::Indent();

    int i = 0;
    for (auto& comp : comps) {
        bool isCompSelectedObj = selectedObjIns.GetSelectedComponent() == comp;

        ImGui::PushID(i);
        // Check if the component is a mesh
        if (std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> modelComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(comp)) {
            if (ImGui::Selectable(ICON_FA_CUBE " Mesh", isCompSelectedObj)) {
                selectedObjIns.SetSelectedComponent(comp);
            }
        }
        // Check if the component is a light
        else if (std::shared_ptr<::Graphics::Primitives::Lights::Light> lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::Light>(comp)) {
            if (ImGui::Selectable(ICON_FA_LIGHTBULB " Light", isCompSelectedObj)) {
                selectedObjIns.SetSelectedComponent(comp);
            }
        }
        else if (std::shared_ptr<Core::Particles::FireSystem> fireComp = std::dynamic_pointer_cast<Core::Particles::FireSystem>(comp)) {
            if (ImGui::Selectable(ICON_FA_FIRE " Fire System", isCompSelectedObj)) {
				selectedObjIns.SetSelectedComponent(comp);
			}
		}
        else if (Core::RTTI::IsA<Graphics::Primitives::Decal>(comp.get())) {
            if (ImGui::Selectable(ICON_FA_NOTE_STICKY " Decal", isCompSelectedObj)) {
                selectedObjIns.SetSelectedComponent(comp);
            }
        }
        else {
            if (ImGui::Selectable(ICON_FA_QUESTION" tipo de comp raro?", isCompSelectedObj)) {
                selectedObjIns.SetSelectedComponent(comp);
            }
        }
        ImGui::PopID();
        i++;

    }
    ImGui::Unindent();
    ImGui::EndChildFrame();
}




void colorPickerBtn(static ImVec4& color) {

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

void CreateSliderRow(const char* rowName, float& sliderValue, float minValue, float maxValue) {
    ImGui::TableSetColumnIndex(0);
    ImGui::Text(rowName);
    ImGui::TableSetColumnIndex(1);

    static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
    std::string SLIDERID = "##" + std::string(rowName);
    ImGui::SliderFloat(SLIDERID.c_str(), &sliderValue, minValue, maxValue, "%.3f");
    ImGui::SameLine();

    auto reloadText = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Core/Editor/Assets/Icons/lockClosed.jpg")->Get();
    ImGui::ImageButton((void*)(intptr_t)reloadText->GetTextureHandle(), ImVec2(13, 13));


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

}

void Properties::LightTransform() {
    std::shared_ptr<Core::Object> obj = selectedObjIns.GetSelectedObject();
    std::shared_ptr<::Graphics::Primitives::Lights::Light> lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::Light>(selectedObjIns.GetSelectedComponent());
	glm::vec3 curPos = lightComp->GetPosition();
	TransformRow("  Location", curPos[0], curPos[1], curPos[2]);
	lightComp->SetPosition(curPos);
}


void Properties::LightingOptions() {

    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

    std::shared_ptr<::Graphics::Primitives::Lights::Light> lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::Light>(selectedObjIns.GetSelectedComponent());

    glm::vec3 color = lightComp->GetColor();
    ImVec4 baseColor = ImVec4(color.x, color.y, color.z, 1.0f);

    if (ImGui::BeginTable("light_table", 2, flags1)) {

        //Light TYPE
        /*
        const char* lightTypes[] = { "SPOT", "POINT", "CCCC" };
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

        */

        //COLOR DE LA LUZ
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Light Color");
        ImGui::TableSetColumnIndex(1);
        colorPickerBtn(baseColor);


        ImGui::EndTable();
    }


    const glm::vec3& colorVec3 = *reinterpret_cast<const glm::vec3*>(&baseColor);
    lightComp->SetColor(colorVec3);

    ImGui::PopStyleVar();



    

}

void Properties::LightTypeOptions(){
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

    if (ImGui::BeginTable("light_type_table", 2, ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH)) {

        // OPCIONES ESPECIFICAS DE CADA TIPO DE LUZ
        std::shared_ptr<::Graphics::Primitives::Lights::Light> lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::Light>(selectedObjIns.GetSelectedComponent());

        if (auto pointLight = std::dynamic_pointer_cast<::Graphics::Primitives::PointLight>(lightComp)) {

            float lightRadius = pointLight->GetRadius();
            float fallOff = pointLight->GetFallOff();


            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Radius");
            ImGui::TableSetColumnIndex(1);
            ImGui::SliderFloat("##LightRadius", &lightRadius, 0.0f, 100.0f, "%.2f");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("FallOff");
            ImGui::TableSetColumnIndex(1);
            ImGui::SliderFloat("##FallOff", &fallOff, 0.0f, 1.0f, "%.2f");

            ImGui::EndTable();

            pointLight->SetRadius(lightRadius);
            pointLight->SetFallOff(fallOff);
        }
        else if (auto directionalLight = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::DirectionalLight>(lightComp)) {

            float dirX = directionalLight->GetDirection().x;
            float dirY = directionalLight->GetDirection().y;
            float dirZ = directionalLight->GetDirection().z;

            ImGui::EndTable();

            TransformRow("  Direction", dirX, dirY, dirZ);

            glm::vec3 normalizedDirection = glm::normalize(glm::vec3(dirX, dirY, dirZ));
            directionalLight->SetDirection(normalizedDirection);

        }
        else {
            auto spotLight = std::dynamic_pointer_cast<::Graphics::Primitives::SpotLight>(lightComp);

            bool shadowCaster = spotLight->GetShadowCasting();
            float lightRadius = spotLight->GetRadius();
            float inner = spotLight->GetInner();
            float outer = spotLight->GetOutter();
            float fallOff = spotLight ->GetFallOff();
            float dirX = spotLight ->GetDirection().x;
            float dirY = spotLight ->GetDirection().y;
            float dirZ = spotLight ->GetDirection().z;

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Shadow Caster");
            ImGui::TableSetColumnIndex(1);
            ImGui::Checkbox("##ShadowCaster", &shadowCaster);

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Radius");
            ImGui::TableSetColumnIndex(1);
            ImGui::SliderFloat("##LightRadius", &lightRadius, 0.0f, 100.0f, "%.2f");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Inner");
            ImGui::TableSetColumnIndex(1);
            ImGui::SliderFloat("##Inner", &inner, 0.0f, 100.0f, "%.2f");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("Outter");
            ImGui::TableSetColumnIndex(1);
            ImGui::SliderFloat("##Outter", &outer, 0.0f, 100.0f, "%.2f");

            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("FallOff");
            ImGui::TableSetColumnIndex(1);
            ImGui::SliderFloat("##FallOff", &fallOff, 0.0f, 1.0f, "%.2f");

            ImGui::EndTable();

            TransformRow("  Direction", dirX, dirY, dirZ);
            spotLight->SetRadius(lightRadius);
            spotLight->SetInner(inner);
            spotLight->SetOuter(outer);
            spotLight->SetFallOff(fallOff);
            spotLight->SetDirection(glm::normalize(glm::vec3(dirX, dirY, dirZ)));
            spotLight->SetShadowCaster(shadowCaster);
        }
    }
    ImGui::PopStyleVar();
}


static std::string nombreTexturaTemporal = "Textura def";

void Properties::MaterialsOptions() {

    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGuiDragDropFlags flags = 0 | ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

    std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(selectedObjIns.GetSelectedComponent());

    auto glbModel = meshComp->GetMesh().lock();
    

    if (glbModel) {
        //std::string directory = glbModel->Get()->getPath();
        //printf("Directory: %s\n", directory.c_str());
    }
    


    if (ImGui::BeginTable("materials_table", 2, flags1)) {

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Element 0");
        ImGui::TableSetColumnIndex(1);


        auto tex = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content\\Textures\\Brick.png")->Get();
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
            auto nuevaTex = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>(iconPtr->ruta)->Get();


        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopStyleVar();


}

void Properties::MeshOptions() {
    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGuiDragDropFlags flags = 0 | ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

    std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(selectedObjIns.GetSelectedComponent());

    auto glbModel = meshComp->GetMesh().lock();

    if (glbModel) {
        //std::string directory = glbModel->Get()->getPath();
        //printf("Directory: %s\n", directory.c_str());
    }

    if (ImGui::BeginTable("mesh_table", 2, flags1)) {

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Element 0");
        ImGui::TableSetColumnIndex(1);


        auto tex = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Core/Editor/Assets/Icons/model.png")->Get();
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
            AssetIcon* asset = (AssetIcon*)payload->Data;
            printf("RUTA MESH: %s\n", asset->ruta);
            meshComp->SetMesh(resmg.GetResource<::Graphics::Primitives::Model>(asset->ruta));
        }
        ImGui::EndDragDropTarget();
    }


    ImGui::PopStyleVar();


}

void Properties::ShaderOptions(Core::Graphics::OpenGLPipeline& pipeline)
{
    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGuiDragDropFlags flags = 0 | ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

    std::shared_ptr<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>> meshComp = std::dynamic_pointer_cast<Core::Graphics::GLBModelRenderer<Core::Graphics::Pipeline::GraphicsAPIS::OpenGL>>(selectedObjIns.GetSelectedComponent());

    
    auto glbModel = meshComp->GetMesh().lock();

    if (glbModel) {
        //std::string directory = glbModel->Get()->getPath();
        //printf("Directory: %s\n", directory.c_str());
    }



    if (ImGui::BeginTable("shader_table", 2, flags1)) {

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Element 0");
        ImGui::TableSetColumnIndex(1);


        auto tex = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Core/Editor/Assets/Icons/shader.png")->Get();
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
            AssetIcon* asset = (AssetIcon*)payload->Data;
            printf("RUTA SHADER PROG: %s\n", asset->ruta);
            //meshComp->SetShaderProgram(resmg.GetResource<Gra>("Content/Shaders/Refractive.shader"));

          
            Core::Assets::Asset<ShaderProgram> newShader = resmg.GetResource<ShaderProgram>(asset->ruta);
            Core::Assets::AssetReference<ShaderProgram> curShaderRef = meshComp->GetShaderProgram();
            Core::Assets::Asset<ShaderProgram> curShader = curShaderRef.lock();;
            
            
            /*
            if (curShaderRef.lock()) 
                curShader = curShaderRef.lock();
            }
            else {
                printf("No hay shader actual\n");
            }
            */

            pipeline.updateRenderablesGroups(curShader, newShader, meshComp);

            
            
            

            meshComp->SetShaderProgram(resmg.GetResource<Core::Graphics::ShaderProgram>("Content/Shaders/White.shader"));
            

           
            //renderer->SetShaderProgram(resmg.GetResource<Core::Graphics::ShaderProgram>(asset->ruta));



           
        }
        ImGui::EndDragDropTarget();
    }


    ImGui::PopStyleVar();
}







void Properties::FireSize() {
    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);

    std::shared_ptr<Core::Particles::FireSystem> fireSystem = std::dynamic_pointer_cast<Core::Particles::FireSystem>(selectedObjIns.GetSelectedComponent());

    glm::vec3 curRadius = fireSystem->GetRadiusVector();
    glm::vec4 color = fireSystem->GetBaseColor();
    ImVec4 baseColor = ImVec4(color.x, color.y, color.z, color.w);

    float curGap = fireSystem->GetFireGap();
    float curHeight = fireSystem->getHeigth();
    float curParticleSize = fireSystem->GetParticleSize();


    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

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

   
    std::shared_ptr<Core::Particles::FireSystem> prevState = fireSystem->getPrevState();

    if (prevState != nullptr) {
        //TODO: SACAR A UNA FUNCION
        bool gapChanged = curGap != prevState->GetFireGap();
        bool heightChanged = curHeight != prevState->getHeigth();
        bool radiusChanged = curRadius != prevState->GetRadiusVector();
        bool particleSizeChanged = curParticleSize != prevState->GetParticleSize();
        bool colorChanged = color != prevState->GetBaseColor();

        if (gapChanged || heightChanged || radiusChanged || particleSizeChanged || colorChanged) {

            fireSystem->ChangeFireSize(curRadius[0], curRadius[1], curRadius[2], curGap, curHeight);
            fireSystem->SetParticleSize(curParticleSize);

            glm::vec4 newColor = glm::vec4(baseColor.x, baseColor.y, baseColor.z, baseColor.w);
            fireSystem->SetBaseColor(newColor);

			fireSystem->saveState();


		}
        
    }
    else {
        fireSystem->saveState();
    }



}


void Properties::UpdateLightCompsPos(std::shared_ptr<Core::Object> obj)
{
    std::vector<std::shared_ptr<Core::Component>> comps = obj->GetAllComponents();
    for (auto& comp : comps) {
        if (std::shared_ptr<::Graphics::Primitives::Lights::Light> lightComp = std::dynamic_pointer_cast<::Graphics::Primitives::Lights::Light>(comp)) {
			glm::vec3 relativePos = lightComp->GetPosition();
			lightComp->SetPosition(relativePos);
		}
	}
}

void Properties::DecalOptions() {
    static ImGuiTableFlags flags1 = ImGuiTableFlags_BordersInner | ImGuiTableFlags_BordersH;
    static ImVec2 cell_padding(4.0f, 8.0f);
    ImGuiDragDropFlags flags = 0 | ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, cell_padding);

    std::shared_ptr<Graphics::Primitives::Decal> decal = std::dynamic_pointer_cast<Graphics::Primitives::Decal>(selectedObjIns.GetSelectedComponent());

    if (ImGui::BeginTable("materials_table", 2, flags1)) {

        ImGui::TableNextRow();

        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Element 0");
        ImGui::TableSetColumnIndex(1);


        auto tex = decal->GetDiffuse().lock()->Get();
        ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));


        ImGui::SameLine();
        ImGui::BeginGroup();


        ImGui::Text(nombreTexturaTemporal.c_str());

        ImGui::Button(ICON_FA_ARROW_TURN_UP);
        ImGui::SameLine();
        ImGui::Button(ICON_FA_FOLDER_OPEN);


        ImGui::EndGroup();

        tex = decal->GetNormal().lock()->Get();
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
            auto nuevaTex = Singleton<Core::Assets::ResourceManager>::Instance().GetResource<Core::Graphics::Texture>(iconPtr->ruta)->Get();


        }
        ImGui::EndDragDropTarget();
    }

    ImGui::PopStyleVar();
}
#include "MainMenu.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include <Dependencies/Json/single_include/json.hpp>
#include "Core/Remote/Client.h"
#include "Core/Remote/Server.h"
#include "Core/Remote/NetManager.h"
#include "Core/AppWrapper.h"
#include "Core/Editor/Editor.h"
#include <stdlib.h>



static bool show_tool_metrics = false;
static bool show_shadow_mapping = false;
static bool show_deferred_rendering = false;





void MainMenu::Render(Core::Graphics::OpenGLPipeline& pipeline) {
    if (ImGui::BeginMainMenuBar()) {

        RenderFileMenu();

        RenderEditMenu();

        if (ImGui::BeginMenu("Debug Tools")) {
            ImGui::MenuItem("Deferred Rendering", NULL, &show_deferred_rendering);
            ImGui::MenuItem("Shadow Mapping", NULL, &show_shadow_mapping);
            ImGui::EndMenu();
        }



        if (ImGui::BeginMenu("Options")) {
            bool antiAlias = pipeline.getAntiAliasing();

            ImGui::SliderFloat("FOV", Singleton<::Editor>::Instance().GetFOV() , 5, 120.0f);

            ImGui::SliderFloat("Exposure", &pipeline.getExposure(), 0, 5);
            if (ImGui::Checkbox("Anti-Aliasing", &antiAlias)) {
                pipeline.setAntiAliasing(antiAlias);
            }

            ImGui::EndMenu();
        }


        RenderRemoteControlMenu();
        RenderActionButtons();
        RenderServerStateInfo();
        if (Singleton<::Editor>::Instance().getIsEditing() || ImGuizmo::IsUsing()) {
            RenderIsSavingState();
        }
        

        ImGui::EndMainMenuBar();

    }


    if (show_deferred_rendering)
        debugTools.RenderDeferredRendering(pipeline);
    if (show_shadow_mapping)
        debugTools.RenderShadowMapping(pipeline);
    if (show_tool_metrics)
        ImGui::ShowMetricsWindow(&show_tool_metrics);

    //ImGui::Text("Hladfa");

    if (savePopup) ImGui::OpenPopup("Save As");

    if (ImGui::BeginPopupModal("Save As", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
        std::cout << "Mostrando popup" << std::endl;
        //ImGui::Text("Filename: ");
        //ImGui::SameLine();
        ImGui::InputTextWithHint("##", "Filename", saveFileName, 100);
        std::cout << "Nombre: " << saveFileName << std::endl;
        int width = ImGui::GetWindowSize().x;
        int buttonHeight = 30;
        int buttonWidth = 100;
        ImGui::Dummy(ImVec2(0, 5));
        ImGui::SetCursorPosX(width / 2 - buttonWidth - 3); //A ojo
        if (ImGui::Button("Save", ImVec2(buttonWidth, buttonHeight))) {
            AppWrapper& app = Singleton<AppWrapper>::Instance();
            //std::cout << saveFileName << std::endl;
            app.getScene().Save("Content/Maps/" + std::string(saveFileName) + ".real");
            savePopup = false;
            previouslySaved = true;
#if _DEBUG
            //Hacer algo
#else
            system("AsssetLoader.exe");
#endif
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, buttonHeight))) {
            savePopup = false;
			ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
    
}

void MainMenu::RenderRemoteControlMenu() {
    if (ImGui::BeginMenu("Remote")) {

        Server& server = Singleton<Server>::Instance();
        Client& client = Singleton<Client>::Instance();

        if (!server.isRunning() && !client.isConnected()) {

            if (ImGui::MenuItem("Host Server", NULL)) {
                server.CreateServer();
            }

            if (ImGui::BeginMenu("Connect to server")) {
                //mandar mensaje de broadcast --> actualiza getServers()
                if (!client.getIsBroadcastBinded()) {
                    std::thread thread(&Client::findServers, &client, 5555);
                    thread.join();
                }
                //lista de IPs disponibles en la red
                std::vector<std::string> servers = client.getServers();

                ImGui::BeginChild("child", ImVec2(0, 100), ImGuiChildFlags_Border);
                for (const std::string server : servers) {
                    ImGui::Text(server.c_str());
                    ImGui::SameLine();
                    if (ImGui::SmallButton("Connect")) {
                        printf("Conectando a %s\n", server.c_str());
                        client.connectToServer(server.c_str(), 5555);
                    }
                }
                ImGui::EndChild();

                //Input Manual de IP
                static char str1[16] = "";
                ImGui::InputTextWithHint("##IPINPUT", "127.0.0.1", str1, IM_ARRAYSIZE(str1));
                ImGui::SameLine();
                if (ImGui::Button("Connect")) {
                    client.connectToServer("127.0.0.1", 5555);
                }
                ImGui::EndMenu();
            }
            else {
                if (client.getIsBroadcastBinded()) {
                    printf("Cerrando escucha de servers...\n");
                    client.closeBroadcastSocket();
                }
            }
        }
        else if (server.isRunning()) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            if (ImGui::MenuItem(ICON_FA_XMARK" Close server", NULL)) {
                server.KillServer();
            }
            ImGui::PopStyleColor();

        }
        else if (client.isConnected()) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
            if (ImGui::MenuItem(ICON_FA_XMARK" Disconnect from server", NULL)) {
                client.disconnectFromServer();
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndMenu();
    }
}

void MainMenu::RenderEditMenu() {
    if (ImGui::BeginMenu("Edit")) {
        size_t numActions = Singleton<::Editor>::Instance().GetActionManager()->GetActions().size();
        int curAction = Singleton<::Editor>::Instance().GetActionManager()->GetCurrentAction();

        if (ImGui::MenuItem("Undo", "CTRL+Z")) {
            std::cout << "Undoing action" << std::endl;
            Singleton<::Editor>::Instance().GetActionManager()->Undo();
        }
        if (ImGui::MenuItem("Redo", "CTRL+Y")) {
            std::cout << "Redoing action" << std::endl;
            Singleton<::Editor>::Instance().GetActionManager()->Redo();
        }
        ImGui::Text((std::to_string(numActions) + " edit actions stored").c_str());
        std::string actionString = "Now at action: [" + std::to_string(curAction) + "]";
        ImGui::Text(actionString.c_str());
        ImGui::EndMenu();
    }
}


void MainMenu::RenderServerStateInfo(){
    Server& server = Singleton<Server>::Instance();
    Client& client = Singleton<Client>::Instance();
    if (server.isRunning()) {
        ImGui::SameLine(ImGui::GetWindowWidth() - 150);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(43.0f / 255.0f, 190.0f / 255.0f, 165.0f / 255.0f, 1.0f));
        ImGui::BeginMenu(ICON_FA_SERVER "  Server runnning");
        ImGui::PopStyleColor();
    }
    else if (client.isConnected()) {
        ImGui::SameLine(ImGui::GetWindowWidth() - 150);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(43.0f / 255.0f, 190.0f / 255.0f, 165.0f / 255.0f, 1.0f));
        ImGui::BeginMenu(ICON_FA_SERVER "  Connected to peer");
        ImGui::PopStyleColor();
    }
}



void MainMenu::RenderActionButtons(){
    float buttonHeight = 20.0f;
    float marginTop = 2.0f;
    ImGuiStyle& style = ImGui::GetStyle();
    
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + marginTop); 

    if (Singleton<::Editor>::Instance().GetActionManager()->canUndo()) {
        if (ImGui::Button(ICON_FA_REPLY"", ImVec2(30, buttonHeight))) {
            Singleton<::Editor>::Instance().GetActionManager()->Undo();
            RenderIsSavingState();
        }
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        ImGui::Button(ICON_FA_REPLY"", ImVec2(30, buttonHeight));
        ImGui::PopStyleVar(); ImGui::PopStyleColor();
    }

    ImGui::SameLine();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + marginTop); 

    if (Singleton<::Editor>::Instance().GetActionManager()->canRedo()) {
        if (ImGui::Button(ICON_FA_SHARE"", ImVec2(30, buttonHeight))) {
            Singleton<::Editor>::Instance().GetActionManager()->Redo();
            RenderIsSavingState();
        }
    }
    else {
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0, 0));
        ImGui::Button(ICON_FA_SHARE"", ImVec2(30, buttonHeight));
        ImGui::PopStyleVar(); ImGui::PopStyleColor();
    }
    ImGui::PopStyleColor();
}

void MainMenu::RenderIsSavingState() {
    int curAction = Singleton<::Editor>::Instance().GetActionManager()->GetCurrentAction();

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); 
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0)); 
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(255.0f / 255.0f, 118.0f / 255.0f, 23.0f / 255.0f, 1.0f));

    std::string actionString = std::to_string(curAction) + " " + " " + ICON_FA_FLAG_CHECKERED;
    ImVec2 textSize = ImGui::CalcTextSize(actionString.c_str());

    ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 50);
    ImGui::TextColored(ImVec4(255.0f / 255.0f, 118.0f / 255.0f, 23.0f / 255.0f, 1.0f), "%s", actionString.c_str());

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::PopFont();
}

   



void MainMenu::RenderFileMenu() {
    if (ImGui::BeginMenu("File")) {
        if (ImGui::MenuItem("Save", "CTRL+S", false, previouslySaved)) {
            AppWrapper& app = Singleton<AppWrapper>::Instance();
            app.getScene().Save("Content/Maps/" + std::string(saveFileName) + ".json");
        }
        if (ImGui::MenuItem("Save As..")) {
            //ImGui::OpenPopup("Save As..");
            savePopup = true;
		}
        ImGui::EndMenu();
    }
}

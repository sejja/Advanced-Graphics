#include "MainMenu.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include <Dependencies/Json/single_include/json.hpp>
#include "Core/Network/Client.h"
#include "Core/Network/Server.h"


static bool show_tool_metrics = false;
static bool show_shadow_mapping = false;
static bool show_deferred_rendering = false;





void MainMenu::Render(Core::Graphics::OpenGLPipeline& pipeline) {
    if (ImGui::BeginMainMenuBar()) {

        if (ImGui::BeginMenu("Edit")) {

            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug Tools")) {
            ImGui::MenuItem("Deferred Rendering", NULL, &show_deferred_rendering);
            ImGui::MenuItem("Shadow Mapping", NULL, &show_shadow_mapping);
            ImGui::EndMenu();
        }



        if (ImGui::BeginMenu("Options")) {
            bool antiAlias = pipeline.getAntiAliasing();

            ImGui::SliderFloat("Exposure", &pipeline.getExposure(), 0, 5);
            if (ImGui::Checkbox("Anti-Aliasing", &antiAlias)) {
                pipeline.setAntiAliasing(antiAlias);
            }
            ImGui::EndMenu();
        }


        RenderRemoteControlMenu();
        ServerStateInfo();

        ImGui::EndMainMenuBar();

    }


    if (show_deferred_rendering)
        debugTools.RenderDeferredRendering(pipeline);
    if (show_shadow_mapping)
        debugTools.RenderShadowMapping(pipeline);
    if (show_tool_metrics)
        ImGui::ShowMetricsWindow(&show_tool_metrics);


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

void MainMenu::ServerStateInfo()
{
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

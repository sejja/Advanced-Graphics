#include "MainMenu.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include <Dependencies/Json/single_include/json.hpp>
#include "Core/Network/Client.h"
#include "Core/Network/Server.h"


static bool show_tool_metrics = false;
static bool show_shadow_mapping = false;
static bool show_deferred_rendering = false;

static bool server_running = false;




void MainMenu::Render(Core::Graphics::OpenGLPipeline& pipeline){
    if (ImGui::BeginMainMenuBar()){


        if (ImGui::BeginMenu("Edit")){

            if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Debug Tools")){

            ImGui::MenuItem("Deferred Rendering", NULL, &show_deferred_rendering);

            ImGui::MenuItem("Shadow Mapping", NULL, &show_shadow_mapping);

            //ImGui::MenuItem("Metrics/Debugger", NULL, &show_tool_metrics);

            ImGui::EndMenu();
        }



        RenderRemoteControlMenu();

        if (server_running) {
            ImGui::SameLine(ImGui::GetWindowWidth() - 150);
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(43.0f / 255.0f, 190.0f / 255.0f, 165.0f / 255.0f, 1.0f));
            ImGui::BeginMenu(ICON_FA_SERVER "  Server runnning");
            ImGui::PopStyleColor();
        }



        ImGui::EndMainMenuBar();

    }

    
    if (show_deferred_rendering)
        debugTools.RenderDeferredRendering(pipeline);
    if (show_shadow_mapping)
		debugTools.RenderShadowMapping(pipeline);
    if (show_tool_metrics)
        ImGui::ShowMetricsWindow(&show_tool_metrics);


}

void MainMenu::RenderRemoteControlMenu(){

    if (ImGui::BeginMenu("Remote")) {

        if (!server_running) {
            ImGui::MenuItem("Host Server", NULL, &server_running);

            if (ImGui::Button("Desconectar")) {
                Client& client = Singleton<Client>::Instance();

                if (client.isConnected()) {
                    client.disconnectFromServer();
                }
            }

            if (ImGui::Button("HOST server asdf")) {
				//connect to server
				Server& server = Singleton<Server>::Instance();
                server.CreateServer();
            }

            if (ImGui::Button("TEST SERVIDORES ABIERTOS")) {
                
                Client& client = Singleton<Client>::Instance();
                client.findServers(5555);

            }

            if (ImGui::Button("ENVIAR")) {
            	//connect to server
                
                
                
                Client& client = Singleton<Client>::Instance();
                // Ejemplo de env�o de mensaje al servidor
                json message;
                message["text"] = "Hola, servidor!";
                client.sendToPeer(message);

                client.sendToPeer(message);


                // Ejemplo de desconexi�n del servidor
                //client.disconnectFromServer();
            
            }

            Client& client = Singleton<Client>::Instance();

            if (ImGui::BeginMenu("Connect to server")) {

                if (!client.getIsBroadcastBinded()){
                    std::thread thread(&Client::findServers, &client, 5555);
                    thread.join();
                }

                std::vector<std::string> servers = client.getServers();

                //Lista de IPs disponibles en red
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
                    //connect to server
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
        else {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::MenuItem("Close server", NULL, &server_running);
			ImGui::PopStyleColor();

        }



     
        ImGui::EndMenu();
    }


}

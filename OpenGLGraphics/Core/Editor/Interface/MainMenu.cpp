#include "MainMenu.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"


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


            if (ImGui::BeginMenu("Connect to server")) {


                //Lista de IPs disponibles en red
                ImGui::BeginChild("child", ImVec2(0, 100), ImGuiChildFlags_Border);
                for (int i = 0; i < 4; i++) {
                    ImGui::Text("127.0.0.%d", i);
                    ImGui::SameLine();
                    ImGui::SmallButton("Connect");
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

        }
        else {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
			ImGui::MenuItem("Close server", NULL, &server_running);
			ImGui::PopStyleColor();

        }



     
        ImGui::EndMenu();
    }


}


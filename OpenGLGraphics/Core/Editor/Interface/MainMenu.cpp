#include "MainMenu.h"


static bool show_tool_metrics = false;
static bool show_shadow_mapping = false;
static bool show_deferred_rendering = false;




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

        ImGui::EndMainMenuBar();

    }

    
    if (show_deferred_rendering)
        debugTools.RenderDeferredRendering(pipeline);
    if (show_shadow_mapping)
		debugTools.RenderShadowMapping(pipeline);
    if (show_tool_metrics)
        ImGui::ShowMetricsWindow(&show_tool_metrics);


}


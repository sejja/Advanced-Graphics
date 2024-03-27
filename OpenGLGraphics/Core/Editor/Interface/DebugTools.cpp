#include "DebugTools.h"
#include "Graphics/Tools/FrameBuffer.h"


void DebugTools::RenderShadowMapping(Core::Graphics::OpenGLPipeline& pipeline){

	if (ImGui::Begin("Shadow Mapping")) {
		int i = 0;
		for (Core::Graphics::FrameBuffer& buff : pipeline.GetShadowMappingBuffer()) {
			ImGui::Image((ImTextureID)buff.GetTextureHandle(),
				ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));

			if (i < 1) {
				ImGui::SameLine();
				i++;
			}
			else
				i = 0;
		}
	}
	ImGui::End();

}

void DebugTools::RenderDeferredRendering(Core::Graphics::OpenGLPipeline& pipeline){

	if (ImGui::Begin("Deferred Rendering")) {
		ImGui::Image((ImTextureID)pipeline.GetGBuffer()->GetPositionTextureHandle(),
			ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::Image((ImTextureID)pipeline.GetGBuffer()->GetAlbedoTextureHandle(),
			ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)pipeline.GetGBuffer()->GetNormalTextureHandle(),
			ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Image((ImTextureID)pipeline.GetBloomTexture(),
			ImVec2(256, 256), ImVec2(0, 1), ImVec2(1, 0));
	}
	ImGui::End();
}

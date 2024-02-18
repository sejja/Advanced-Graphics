#include "AssetIcon.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/Singleton.h"
#include "Core/ResourceManager.h"
#include "Graphics/Primitives/Texture.h"


AssetIcon::AssetIcon(std::string p_tipo, std::string p_nombre, std::string p_ruta)
{
	tipo = p_tipo;
	nombre = p_nombre;
	ruta = p_ruta;
}


AssetIcon::AssetIcon() {
	
}

void AssetIcon::dibujar() {
	//ImGui::Image()
	ImGui::BeginGroup();
	//ImGui::Button("Hola", ImVec2(100, 100));

	auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Content/Textures/back.jpg")->Get(); //Que cojones es la sintaxis de esta línea???
	ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(100, 100)); //No funciona

	int centerOffset = 100 / 2 - ImGui::CalcTextSize("nombre").x / 2;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerOffset);
	ImGui::Text("nombre");
	ImGui::EndGroup();

}


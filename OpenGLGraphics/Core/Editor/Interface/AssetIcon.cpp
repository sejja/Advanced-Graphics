#include "AssetIcon.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/Singleton.h"
#include "Core/ResourceManager.h"
#include "Graphics/Primitives/Texture.h"




//Write a constructor that takes all the parameters
//AssetIcon::AssetIcon(AssetType p_tipo, std::string p_nombre, std::string p_ruta) {
//	tipo = p_tipo;
//	nombre = p_nombre;
//	ruta = p_ruta;
//}


AssetIcon::AssetIcon() {
	tipo = AssetType::OTHER;
	nombre = "Sin nombre";
	ruta = "";
}

AssetIcon::AssetIcon(AssetType p_tipo, const char* p_nombre, const char* p_ruta)
{
	tipo = p_tipo;
	nombre = p_nombre;
	ruta = p_ruta;
}

void AssetIcon::dibujar() {
	//ImGui::Image()
	ImGui::BeginGroup();
	//ImGui::Button("Hola", ImVec2(100, 100));

	if (tipo == AssetType::MODEL) {
		auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Core/Editor/Assets/Icons/folderAdd.png")->Get(); //Que cojones es la sintaxis de esta línea???
		ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0)); //Por algún motivo se carga al reves, creo que es lo del vídeo
	}
	//else if (tipo == "texture") {
	//	auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>(ruta)->Get(); //Que cojones es la sintaxis de esta línea???
	//	ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(100, 100));
	// 
	//}
	else {
		auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>("Core/Editor/Assets/Icons/other.png")->Get(); //Que cojones es la sintaxis de esta línea???
		ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
	}
	



	int centerOffset = 100 / 2 - ImGui::CalcTextSize(nombre).x / 2;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerOffset);
	ImGui::Text(nombre);
	ImGui::EndGroup();

}


#include "asset.h"
#include "Dependencies/ImGui/imgui.h"
#include "Core/Singleton.h"
//#include "Core/ResourceManager.h"


Asset::Asset(std::string p_tipo, std::string p_nombre, std::string p_ruta)
{
	tipo = p_tipo;
	nombre = p_nombre;
	ruta = p_ruta;
}

Asset::Asset() {
	
}

void Asset::dibujar() {
	//ImGui::Image()
	ImGui::BeginGroup();
	ImGui::Button("Hola", ImVec2(100, 100));

	//auto tex = Singleton<ResourceManager>::Instance().GetResource<Texture>("Assets/Icons/folderAdd.png")->Get(); //Que cojones es la sintaxis de esta línea???

	int centerOffset = 100/2 - ImGui::CalcTextSize("nombre").x / 2;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerOffset);
	ImGui::Text("nombre");
	ImGui::EndGroup();

}
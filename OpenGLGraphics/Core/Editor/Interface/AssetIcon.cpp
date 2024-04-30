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
	clicked = false;
}

AssetIcon::AssetIcon(AssetType p_tipo, const char* p_nombre, const char* p_ruta)
{
	tipo = p_tipo;
	nombre = p_nombre;
	ruta = p_ruta;
	clicked = false;
	//printf("%p\n", nombre);
	printf("%s, %s\n", nombre, ruta);
}

void AssetIcon::dibujar(bool dibujarToolTip) { //Probablemente sea más eficiente no llamar recursivamente a esta función
	//ImGui::Image()
	ImGui::BeginGroup();
	ImGui::PushID(ruta);
	//clicked = false;
	//ImGui::Button("Hola", ImVec2(100, 100));

	if (tipo == AssetType::TEXTURE) {
		auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>(ruta)->Get(); //Que cojones es la sintaxis de esta línea???
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
		clicked = ImGui::IsItemClicked();
		ImGui::PopStyleColor(1);
		if (dibujarToolTip == false) {
			bool dragDrop = ImGui::BeginDragDropSource();
			printf("Dragdrop: %d\n", dragDrop);
			if (dragDrop) {
				//printf("Dragging");
				dibujar(true);
				ImGui::SetDragDropPayload("other", this, sizeof(AssetIcon));
				printf("nombre: %s\n", this->nombre);
				ImGui::EndDragDropSource();
			}
		}
		//if (dibujarToolTip == false) {
		//	if (ImGui::BeginDragDropSource()) {
		//		//printf("Dragging");
		//		dibujar(true);
		//		ImGui::SetDragDropPayload("other", this, sizeof(AssetIcon));
		//		printf("nombre: %s\n", this->nombre);
		//		ImGui::EndDragDropSource();
		//	}
		//}
	 
	}
	else {
		auto tex = Singleton<ResourceManager>::Instance().GetResource<Core::Graphics::Texture>(assetTypeImages[tipo].c_str())->Get(); //Que cojones es la sintaxis de esta línea???
		//ImGui::Image((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
		//ImGui::Button("Hola", ImVec2(100, 100));
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		clicked = ImGui::ImageButton((void*)(intptr_t)tex->GetTextureHandle(), ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleColor(1);
		if (dibujarToolTip == false) {
			//bool dragDrop = ImGui::BeginDragDropSource();
			//printf("Dragdrop: %d\n", dragDrop);
			if (ImGui::BeginDragDropSource()) {
				//printf("Dragging");
				dibujar(true);
				ImGui::SetDragDropPayload("other", this, sizeof(AssetIcon));
				printf("nombre: %s\n", this->nombre);
				ImGui::EndDragDropSource();
			}
		}
		//Vale, el error de drag and drop tiene algo que ver con las texturas, ya que cuando se hace drag imgui cree que se hace drag en todos
		// los botones que tienen esa textura
		
		
		
	}

	/*clicked = ImGui::IsItemClicked();*/
	if (clicked == true) {
		printf(nombre);
	}

	//printf("%d\n",clicked);
	ImGui::PopID();
	int centerOffset = 100 / 2 - ImGui::CalcTextSize(nombre).x / 2;
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + centerOffset);
	//printf("%p\n", nombre);
	//printf("%s\n", nombre);
	ImGui::Text(nombre);
	ImGui::EndGroup();

}

std::unordered_map<AssetType, std::string> AssetIcon::assetTypeImages;


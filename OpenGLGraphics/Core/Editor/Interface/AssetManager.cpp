#include "AssetManager.h"
#include "Dependencies/ImGui/imgui.h"
#include "AssetIcon.h"


void drawDropWindow();
std::string texto = "No se ha droppeado nada";

void AssetManager::Render() {
	ImGui::Begin("Asset Manager");

	ImVec2 btSize = ImVec2(100, 100);
	int numeroElementosFila = elementosPorFila(ImGui::GetWindowWidth(), 100);
	//printf("%d", ImGui::GetWindowWidth());

	const int numAssets = 20;
	AssetIcon assets[20];
	assets[1] = AssetIcon(AssetType::TEXTURE, "Textura", "Content\\Textures\\Brick.png");

	int cont = 0;
	for (int i = 0; i < numAssets; i++) { //TODO intentar centrar elementos
		//ImGui::Button("Boton", btSize);
		assets[i].dibujar(false);
		cont++;
		//printf("%d;%d   ", numeroElementosFila, cont);
		if (cont != numeroElementosFila)
		{
			ImGui::SameLine();
		}
		else
		{
			ImGui::Dummy(ImVec2(0, 20)); //Espacio en blanco vertical
			cont = 0;
		}
	}

	ImGui::NewLine();
	ImGui::End();

	drawDropWindow();

	
}

int AssetManager::elementosPorFila(int anchoVentana, int anchoElemento)
{
	int offset = 8;
	return anchoVentana / (anchoElemento + offset);
}

//Write a method to make a ImGui window with a button of 300*300 size
void drawDropWindow() { //Para eliminar en la verrsion final
	ImGui::Begin("Drop Window");
	ImGui::Button("Drop", ImVec2(300, 300));
	//std::string texto = "No se ha droppeado nada";
	ImGuiDragDropFlags flags = 0;
	flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
	if (ImGui::BeginDragDropTarget()) {
		printf("Dropping");
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("other", flags)) {
			AssetIcon* iconPtr = (AssetIcon*) payload->Data;
			texto = iconPtr->nombre;
		}
		ImGui::EndDragDropTarget();
	}
	ImGui::Text(texto.c_str());
	ImGui::End();
}
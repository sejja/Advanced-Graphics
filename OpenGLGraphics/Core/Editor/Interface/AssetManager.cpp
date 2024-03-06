#include "AssetManager.h"
#include "Dependencies/ImGui/imgui.h"
#include "AssetIcon.h"
#include "../database.h"
#include <vector>


void drawDropWindow();
std::string texto = "No se ha droppeado nada";

AssetManager::AssetManager() {
	printf("Intentado abrir base de datos\n");
	//Core::Editor::Database db("database.db");
	this->db = new Core::Editor::Database("database.db");
	//assets = db.getFilesOfFolder("Ruta");
	assets = db->getFilesOfRoot();
	//db.closeConnection();
}

AssetManager::~AssetManager() {
	db->closeConnection();
}

void AssetManager::Render() {
	ImGui::Begin("Asset Manager");

	ImVec2 btSize = ImVec2(100, 100);
	int numeroElementosFila = elementosPorFila(ImGui::GetWindowWidth(), 100);
	//printf("%d", ImGui::GetWindowWidth());

	//const int numAssets = 20;
	//AssetIcon assets[20];
	//assets[1] = AssetIcon(AssetType::TEXTURE, "Textura", "Content\\Textures\\Brick.png");

	int cont = 0;
	for (int i = 0; i < assets.size(); i++) { //TODO intentar centrar elementos
		//ImGui::Button("Boton", btSize);
		assets[i].dibujar(false);
		if (assets[i].clicked) {
			switch (assets[i].tipo)
			{
			case AssetType::FOLDER:
				printf("Carpeta\n");
				printf("%s\n", assets[i].ruta);
				assets = db->getFilesOfFolder(assets[i].ruta);
				break;
			default:
				printf("Click\n");
				break;
			}
			//printf("Click\n");
		}
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
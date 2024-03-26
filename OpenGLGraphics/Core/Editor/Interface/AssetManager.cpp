#include "AssetManager.h"
#include "Dependencies/ImGui/imgui.h"
#include "AssetIcon.h"
#include "../database.h"
#include <vector>
#include "../Editor.h"
#include "Core/Editor/Assets/Fonts/IconsFontAwesome.h"
#include "Core/Editor/Interface/TextEditor.h"


void drawDropWindow();
std::string texto = "No se ha droppeado nada";

AssetManager::AssetManager() {
	printf("Intentado abrir base de datos\n");
}

void AssetManager::Render() {
	ImGui::Begin("Asset Manager");

	ImVec2 btSize = ImVec2(100, 100);
	int numeroElementosFila = elementosPorFila(ImGui::GetWindowWidth(), 100); //Esto se puede optimizar
	//printf("%d", ImGui::GetWindowWidth());

	//const int numAssets = 20;
	//AssetIcon assets[20];
	//assets[1] = AssetIcon(AssetType::TEXTURE, "Textura", "Content\\Textures\\Brick.png");


	//Dibujar el menú para regresar
	ImGui::BeginGroup();
	//ImGui::PushStyleColor(ImGuiCol_);
	if (ImGui::Button(ICON_FA_ARROW_UP, ImVec2(25, 25))) {
		gotoPreviousFolder();
	}
	ImGui::SameLine();
	if (ImGui::Button(ICON_FA_HOUSE, ImVec2(25, 25))) {
		init();
	}

	ImGui::SameLine();
	ImGui::BeginGroup();
	ImGui::Dummy(ImVec2(0,1));
	ImGui::Text(folder.ruta);
	ImGui::EndGroup();

	ImGui::EndGroup();

	ImGui::Separator();

	ImGui::BeginGroup();
	int cont = 0;
	for (int i = 0; i < assets.size(); i++) { //TODO intentar centrar elementos
		//ImGui::Button("Boton", btSize);
		assets[i].dibujar(false);
		if (assets[i].clicked) {
			printf("Click\n");
			switch (assets[i].tipo)
			{
			case AssetType::FOLDER:
				//printf("Capreatae");
				changeDirectory(i);
				break;
			case AssetType::SHADER:
				Singleton<Editor>::Instance().texteditor.ChangeFile(assets[i].ruta);
				//printf(assets[i].ruta);
				break;
			default:
				
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
	ImGui::EndGroup();
	ImGui::End();

	drawDropWindow();

	
}

void AssetManager::changeDirectory(int i)
{
	//printf("Carpeta\n");
	previousFolders.push(folder);
	folder = assets[i];
	//printf("%s\n", assets[i].ruta);
	//assets = editor->database->getFilesOfFolder(assets[i].ruta);
	// Aquí queda memoria sin liberar??
	assets = Singleton<Editor>::Instance().database->getFilesOfFolder(assets[i].ruta);
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

void AssetManager::init() {
	assets = Singleton<Editor>::Instance().database->getFilesOfRoot();
	folder = AssetIcon(); //Esto cambiar
	folder.ruta = "Core";
}

void AssetManager::gotoPreviousFolder() {
	switch (previousFolders.size()) {
	case 0:
		break;
	case 1:
		folder = previousFolders.top();
		previousFolders.pop();
		assets = Singleton<Editor>::Instance().database->getFilesOfRoot();
		break;
	default:
		folder = previousFolders.top();
		previousFolders.pop();
		assets = Singleton<Editor>::Instance().database->getFilesOfFolder(folder.ruta);
		break;
	}
	
}
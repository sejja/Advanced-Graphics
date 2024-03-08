#include "Server.h"
#include <iostream>
#include <string>


#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ServerReceiverThread(LPVOID lpParam) {
    SOCKET clientSocket = *((SOCKET*)lpParam);
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Received from client: " << buffer << std::endl;
        }
    }
}

DWORD WINAPI StartServerThread(LPVOID lpParam) {
    Server* serverInstance = static_cast<Server*>(lpParam);
    if (serverInstance)
        return serverInstance->StartServer();
    return 1; //ERROR
}

int Server::CreateServer() {
    HANDLE serverThread = CreateThread(NULL, 0, StartServerThread, this, 0, NULL);
    if (serverThread == NULL) {
        std::cerr << "Failed to create server thread." << std::endl;
        return 1;
    }
    // CloseHandle(serverThread);
    return 0;
}

int Server::StartServer() {
    if (serverSocket != INVALID_SOCKET) {
        KillServer();
    }

    int clientAddrSize = sizeof(clientAddr);

    // Inicializar conexión
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Crear socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }

    // Configurar estructura de dirección
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(serverPort);

    // Enlazar socket
    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Socket bind failed." << std::endl;
        return 1;
    }

    // Escuchar conexiones entrantes
    if (listen(serverSocket, 5) == SOCKET_ERROR) {
        std::cerr << "Listen failed." << std::endl;
        return 1;
    }

    std::cout << "Server is listening..." << std::endl;

    // Aceptar una conexión de un cliente
    if ((clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
        std::cerr << "Accept failed." << std::endl;
        return 1;
    }

    std::cout << "Client connected." << std::endl;

    // Crear hilo para recibir mensajes del cliente
    HANDLE receiverThread = CreateThread(NULL, 0, ServerReceiverThread, &clientSocket, 0, NULL);

    return 0;
}

void Server::KillServer() {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

void Server::sendToClient(const json& message) {
    std::string serialized_message = message.dump();
    send(clientSocket, serialized_message.c_str(), serialized_message.length(), 0);
}


void Server::sendObjectIfChanged(const std::shared_ptr<Core::Object>& obj) {

    glm::vec3 curPos = obj->GetPosition();
    glm::vec3 curRot = obj->GetRotation();
    glm::vec3 curScale = obj->GetScale();

    
    bool positionChanged = true;
    bool rotationChanged = true;
    bool scaleChanged = true;


    if (lastSentObject != NULL) {
        positionChanged = curPos != lastSentObject->GetPosition();
        rotationChanged = curRot != lastSentObject->GetRotation();
        scaleChanged = curScale != lastSentObject->GetScale();
	}

    
    if (positionChanged || rotationChanged || scaleChanged) {
      
        printf("Sending object properties to client\n");
        json data = {
            {"type", "object_transform"},
            {"id", obj->GetID()},
            {"position", {curPos.x, curPos.y, curPos.z}},
            {"rotation", {curRot.x, curRot.y, curRot.z}},
            {"scale", {curScale.x, curScale.y, curScale.z}}
        };

        sendToClient(data);

        lastSentObject = std::make_shared<Core::Object>(*obj);
    }
    
}
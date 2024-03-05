#include "Client.h"
#include <iostream>
#include <string>


#pragma comment(lib, "ws2_32.lib")

DWORD WINAPI ReceiverThread(LPVOID lpParam) {
    SOCKET clientSocket = *((SOCKET*)lpParam);
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }
    }
}


void Client::connectToServer(const char* ip, int port){
    WSADATA WSAData;
    struct sockaddr_in serverAddr;

    // Inicializar conexión
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return;
    }
    //Crear el sockt
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);

    // Conectarse al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection to server failed." << std::endl;
        return;
    }

    std::cout << "Connected to server." << std::endl;

    // Crear hilo para recibir mensajes del servidor
    HANDLE receiverThread = CreateThread(NULL, 0, ReceiverThread, &clientSocket, 0, NULL);

}

void Client::disconnectFromServer(){
	closesocket(clientSocket);
	WSACleanup();
	std::cout << "Disconnected from server." << std::endl;
}

void Client::sendToServer(const json& message){
    std::string serialized_message = message.dump();
    send(clientSocket, serialized_message.c_str(), serialized_message.length(), 0);
}





DWORD __stdcall Client::receiveFromServer(LPVOID lpParam){
    SOCKET clientSocket = *((SOCKET*)lpParam);
    char buffer[1024];
    int bytesReceived;
    while (true) {
        bytesReceived = recv(clientSocket, buffer, 1024, 0);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            json received_message = json::parse(buffer);
            std::cout << "Received: " << received_message["text"] << std::endl;
        }
        else if (bytesReceived == 0) {
            std::cout << "Server disconnected." << std::endl;
            break;
        }
        else {
            std::cerr << "Receive failed." << std::endl;
            break;
        }
    }
    return 0;
}


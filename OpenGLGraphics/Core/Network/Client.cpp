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
    HANDLE receiverThread = CreateThread(NULL, 0, ReceiveThread, &clientSocket, 0, NULL);

}

void Client::disconnectFromServer(){
	closesocket(clientSocket);
	WSACleanup();
	std::cout << "Disconnected from server." << std::endl;
}



void Client::ListenForServers() {
    // Crear un socket de broadcast UDP
    SOCKET broadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (broadcastSocket == INVALID_SOCKET) {
        std::cerr << "Broadcast socket creation failed." << std::endl;
        return;
    }

    // Configurar la dirección de escucha
    sockaddr_in listenAddr;
    memset(&listenAddr, 0, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(50000); // Puerto de broadcast UDP
    listenAddr.sin_addr.s_addr = INADDR_ANY;

    // Enlazar el socket al puerto de escucha
    if (bind(broadcastSocket, (sockaddr*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR) {
        std::cerr << "Broadcast socket bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(broadcastSocket);
        return;
    }
    isBroadcastBinded = true;
    

    // Escuchar mensajes de broadcast
    sockaddr_in serverAddr;
    char recvBuffer[1024];
    int serverAddrLen = sizeof(serverAddr);
    while (true) {
        int bytesReceived = recvfrom(broadcastSocket, recvBuffer, sizeof(recvBuffer), 0, (sockaddr*)&serverAddr, &serverAddrLen);
        if (bytesReceived > 0) {
            recvBuffer[bytesReceived] = '\0';
            std::cout << "Received broadcast message from server: " << recvBuffer << std::endl;
            char ipBuffer[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &(serverAddr.sin_addr), ipBuffer, INET_ADDRSTRLEN);
            std::cout << "Server IP: " << ipBuffer << ", Port: " << ntohs(serverAddr.sin_port) << std::endl;
            //insertar la ip en availableServers
            std::string serverIp = ipBuffer;
            availableServers.push_back(serverIp);

        }
    }

    // Cerrar el socket de broadcast
    closesocket(broadcastSocket);
}


void Client::findServers(int port) {
    std::thread listenThread(&Client::ListenForServers, this);
    listenThread.detach();
}








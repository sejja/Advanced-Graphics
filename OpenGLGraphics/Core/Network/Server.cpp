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

void Server::BroadcastServerPresence() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return;
    }
    printf("WSA iniciado\n");

    broadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (broadcastSocket == INVALID_SOCKET) {
        std::cerr << "Broadcast socket creation failed." << WSAGetLastError() << std::endl;
        return;
    }

    // Configurar la dirección de escucha
    sockaddr_in listenAddr;
    memset(&listenAddr, 0, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(50000); // Puerto de broadcast UDP TEMP HARCODEDADO
    listenAddr.sin_addr.s_addr = INADDR_ANY;

    // Enlazar el socket 
    if (bind(broadcastSocket, (sockaddr*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR) {
        std::cerr << "Broadcast socket bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(broadcastSocket);
        return;
    }

    // Escuchar mensajes del broadcast
    sockaddr_in clientAddr;
    char recvBuffer[1024];
    int clientAddrLen = sizeof(clientAddr);
    while (true) {
        int bytesReceived = recvfrom(broadcastSocket, recvBuffer, sizeof(recvBuffer), 0, (sockaddr*)&clientAddr, &clientAddrLen);
        if (bytesReceived > 0) {
            recvBuffer[bytesReceived] = '\0';
            std::cout << "Received broadcast message from client: " << recvBuffer << std::endl;
            // Responder al cliente 
            const char* responseMessage = "Server available";
            if (sendto(broadcastSocket, responseMessage, strlen(responseMessage), 0, (sockaddr*)&clientAddr, sizeof(clientAddr)) == SOCKET_ERROR) {
                std::cerr << "Broadcast response send failed." << std::endl;
            }
        }
    }

    // Cerrar el socket de broadcast
    closesocket(broadcastSocket);
}

int Server::CreateServer() {

    // Crear un hilo para enviar mensajes de broadcast
    std::thread broadcastThread(&Server::BroadcastServerPresence, this);
    broadcastThread.detach(); // Desconectar el hilo principal del hilo de broadcast


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

    // Inicializar conexi?n
    if (WSAStartup(MAKEWORD(2, 2), &WSAData) != 0) {
        std::cerr << "WSAStartup failed." << std::endl;
        return 1;
    }

    // Crear socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation failed." << std::endl;
        return 1;
    }

    // Configurar estructura de direcci?n
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

    // Aceptar una conexi?n de un cliente
    if ((clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddr, &clientAddrSize)) == INVALID_SOCKET) {
        std::cerr << "Accept failed." << std::endl;
        return 1;
    }

    std::cout << "Client connected." << std::endl;

    // Crear hilo para recibir mensajes del cliente
    HANDLE receiverThread = CreateThread(NULL, 0, ReceiveThread, &clientSocket, 0, NULL);

    return 0;
}

void Server::KillServer() {
    closesocket(clientSocket);
    closesocket(serverSocket);
    closesocket(broadcastSocket);
    WSACleanup();
    serverSocket = NULL;
}


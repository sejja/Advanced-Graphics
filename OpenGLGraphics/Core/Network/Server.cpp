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
    // Crear un socket de broadcast UDP
    SOCKET broadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (broadcastSocket == INVALID_SOCKET) {
        std::cerr << "Broadcast socket creation failed." << WSAGetLastError() << std::endl;
        return;
    }

    // Habilitar el broadcast en el socket
    BOOL broadcastEnabled = TRUE;
    if (setsockopt(broadcastSocket, SOL_SOCKET, SO_BROADCAST, (char*)&broadcastEnabled, sizeof(broadcastEnabled)) == SOCKET_ERROR) {
        std::cerr << "Broadcast socket option set failed." << std::endl;
        closesocket(broadcastSocket);
        return;
    }

    // Configurar la dirección de broadcast
    sockaddr_in broadcastAddr;
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(50000); // Puerto de broadcast UDP
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;

    // Mensaje de broadcast
    const char* broadcastMessage = "Server available";

    // Enviar periódicamente el mensaje de broadcast
    while (true) {
        if (sendto(broadcastSocket, broadcastMessage, strlen(broadcastMessage), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr)) == SOCKET_ERROR) {
            std::cerr << "Broadcast message send failed." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Enviar el mensaje cada 5 segundos
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
    HANDLE receiverThread = CreateThread(NULL, 0, ReceiveThread, &clientSocket, 0, NULL);

    return 0;
}

void Server::KillServer() {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}



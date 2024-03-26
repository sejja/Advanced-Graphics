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


void Client::connectToServer(const char* ip, int port) {
    WSADATA WSAData;
    struct sockaddr_in serverAddr;

    // Inicializar conexi?n
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

void Client::disconnectFromServer() {
    closesocket(clientSocket);
    WSACleanup();
    std::cout << "Disconnected from server." << std::endl;
}



void Client::ListenForServers() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed with error: " << result << std::endl;
        return;
    }

    // Crear un socket de broadcast UDP
    broadcastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (broadcastSocket == INVALID_SOCKET) {
        std::cerr << "Broadcast socket creation failed." << WSAGetLastError() << std::endl;
        return;
    }

    // Configurar la direcci?n de escucha
    sockaddr_in listenAddr;
    const char* requestMsg = "GET_AWAKE_SERVERS";
    sockaddr_in broadcastAddr;
    memset(&listenAddr, 0, sizeof(listenAddr));
    listenAddr.sin_family = AF_INET;
    listenAddr.sin_port = htons(50001); // Puerto de broadcast UDP
    listenAddr.sin_addr.s_addr = INADDR_ANY;

    // Enlazar el socket al puerto de escucha
    if (bind(broadcastSocket, (sockaddr*)&listenAddr, sizeof(listenAddr)) == SOCKET_ERROR) {
        std::cerr << "Broadcast socket bind failed with error: " << WSAGetLastError() << std::endl;
        closesocket(broadcastSocket);
        return;
    }
    isBroadcastBinded = true;


    // Establecer la opci?n SO_BROADCAST
    int broadcastOption = 1;
    if (setsockopt(broadcastSocket, SOL_SOCKET, SO_BROADCAST, (const char*)&broadcastOption, sizeof(broadcastOption)) == SOCKET_ERROR) {
        std::cerr << "Failed to set broadcast socket option with error: " << WSAGetLastError() << std::endl;
        closesocket(broadcastSocket);
        return;
    }

    // Configurar la direcci?n de broadcast

    memset(&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_port = htons(50000); // Puerto de broadcast UDP
    broadcastAddr.sin_addr.s_addr = INADDR_BROADCAST;

    int bytesSent = sendto(broadcastSocket, requestMsg, strlen(requestMsg), 0, (sockaddr*)&broadcastAddr, sizeof(broadcastAddr));
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Failed to send broadcast message." << WSAGetLastError() << std::endl;
        closesocket(broadcastSocket);
        return;
    }
    printf("Broadcast message sent.\n");


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
            //relodeamos el vector de servidores disponibles
            availableServers.clear();
            availableServers.push_back(serverIp);

        }
    }

    // Cerrar el socket de broadcast
    closesocket(broadcastSocket);
}

void Client::closeBroadcastSocket() {
    closesocket(broadcastSocket);
    isBroadcastBinded = false;
    std::cout << "Broadcast socket closed." << std::endl;
    //WSACleanup();
}


void Client::findServers(int port) {
    std::thread listenThread(&Client::ListenForServers, this);
    listenThread.detach();
}







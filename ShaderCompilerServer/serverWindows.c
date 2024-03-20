#include "serverWindows.h"

void receiveFile(SOCKET connfd) {
    char* buffer = malloc(MAX);
    FILE* fp;

    if (buffer == NULL) {
        printf("Error allocating memory for buffer.\n");
        return;
    }

    if (fopen_s(&fp, "received_file.frag", "wb") != 0) {
        printf("Error opening file for writing.\n");
        free(buffer);
        return;
    }

    // Receive file contents from client and write to file
    int n;
    (n = recv(connfd, buffer, MAX, 0));
    if (fwrite(buffer, 1, n, fp) < n) {
        printf("Error writing to file.\n");
        if (ferror(fp)) {
            perror("fwrite");
        }
    }
    if (n < 0) {
        printf("Error receiving file.\n");
    }
    else if (n == 0) {
        printf("Connection closed by client.\n");
    }

    fclose(fp);
    free(buffer);
}

void modifyAndSendFile(SOCKET connfd) {
    char* buffer = malloc(MAX);
    FILE* fp;

    system("C://VulkanSDK//1.3.275.0//Bin//glslangValidator -G -o received_file.spv  received_file.frag");
    Sleep(1000);

    if (fopen_s(&fp, "received_file.spv", "rb") != 0) { // Open file in binary read mode
        printf("Error opening file.\n");
        free(buffer);
        return;
    }

    // Read the entire file into the buffer
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (file_size > MAX) {
        printf("File size exceeds buffer capacity.\n");
        fclose(fp);
        free(buffer);
        return;
    }

    if (fread(buffer, 1, file_size, fp) != file_size) {
        printf("Error reading file.\n");
        fclose(fp);
        free(buffer);
        return;
    }

    // Send the buffer over the connection
    if (send(connfd, buffer, file_size, 0) < 0) {
        printf("Error sending file.\n");
    }

    // Close the file
    fclose(fp);
    free(buffer);
}

int main() {
    setlocale(LC_ALL, "");
    WSADATA wsaData;
    SOCKET sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("socket creation failed...\n");
        WSACleanup();
        return 1;
    }
    else
        printf("Socket successfully created..\n");

    memset(&servaddr, 0, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("socket bind failed...\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if (listen(sockfd, 5) != 0) {
        printf("Listen failed...\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
        printf("Server listening..\n");

    int len = sizeof(cli);

    // Accept the data packet from client and verification
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len);
    if (connfd == INVALID_SOCKET) {
        printf("server accept failed...\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else {
        printf("server accept the client...\n");
    }

    while (1) {
        // Function to receive file from client
        receiveFile(connfd);
        // Function to modify file and send it back to client
        modifyAndSendFile(connfd);
        printf("File sent back to client.\n");


        //closesocket(sockfd);
        //closesocket(connfd);
        //WSACleanup();
        //clear the buffer
        printf("end");
        Sleep(2000);
    }
    return 0;
}

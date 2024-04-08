#include "serverWindows.h"

// Function to receive a file from the client
// Parameters:
// - connfd: Socket descriptor for the connection with the client
void receiveFile(SOCKET connfd) {
    char* buffer = malloc(MAX);   // Buffer to store received file contents
    FILE* fp;                      // File pointer

    // Check for memory allocation error
    if (buffer == NULL) {
        printf("Error allocating memory for buffer.\n");
        return;
    }

    // Attempt to open the file in binary write mode
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

    // Close the file and free the buffer
    fclose(fp);
    free(buffer);
}

// Function to modify a file and send it back to the client
// Parameters:
// - connfd: Socket descriptor for the connection with the client
void modifyAndSendFile(SOCKET connfd) {
    char* buffer = malloc(MAX);   // Buffer to store file contents
    FILE* fp;                      // File pointer

    // Execute external command to modify the file
    system("C://VulkanSDK//1.3.275.0//Bin//glslangValidator -G -o received_file.spv  received_file.frag");
    Sleep(1000);

    // Attempt to open the modified file in binary read mode
    if (fopen_s(&fp, "received_file.spv", "rb") != 0) {
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

    // Close the file and free the buffer
    fclose(fp);
    free(buffer);
}

// Main function
int main() {
    setlocale(LC_ALL, "");  // Set locale to support Unicode characters
    WSADATA wsaData;         // Structure for Winsock data
    SOCKET sockfd, connfd;   // Socket descriptors
    struct sockaddr_in servaddr, cli;  // Structures for server and client socket addresses

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("socket creation failed...\n");
        WSACleanup();
        return 1;
    }
    else
        printf("Socket successfully created..\n");

    memset(&servaddr, 0, sizeof(servaddr));

    // Set up server socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        printf("socket bind failed...\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
        printf("Socket successfully binded..\n");

    // Listen for incoming connections
    if (listen(sockfd, 5) != 0) {
        printf("Listen failed...\n");
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
        printf("Server listening..\n");

    int len = sizeof(cli);

    // Accept incoming connection
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

        // Clear the buffer and wait for a brief period
        printf("end");
        Sleep(2000);
    }
    return 0;
}

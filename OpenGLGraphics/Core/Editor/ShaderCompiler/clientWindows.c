#include "clientWindows.h"

// Function to change the file extension of a given file path
// Parameters:
// - myStr: The file path to modify
// Returns:
// - A newly allocated string with the modified file path
char* changeExtension(char* myStr) {
    char* retStr;    // Pointer to the modified file path
    char* lastExt;   // Pointer to the last occurrence of '.'

    // Check if input string is NULL or memory allocation fails
    if (myStr == NULL || (retStr = malloc(strlen(myStr) + 5)) == NULL)
        return NULL;

    // Copy the input string to the modified string
    strcpy(retStr, myStr);

    // Find the last occurrence of '.' in the file path
    lastExt = strrchr(retStr, '.');

    // If a file extension exists, remove it
    if (lastExt != NULL)
        *lastExt = '\0';

    // Append ".spv" as the new file extension
    strcat(retStr, ".spv");

    // Return the modified file path
    return retStr;
}

// Function to send a file over a network socket
// Parameters:
// - sockfd: The socket descriptor
// - filePath: The path to the file to send
void sendFile(SOCKET sockfd, const char* filePath) {
    char* buffer = malloc(MAX);   // Buffer to store file contents
    FILE* fp;                      // File pointer

    // Attempt to open the file in binary read mode
    if (fopen_s(&fp, filePath, "rb") != 0) {
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
    if (send(sockfd, buffer, file_size, 0) < 0) {
        printf("Error sending file.\n");
    }


    // Close the file and free the buffer
    fclose(fp);
    free(buffer);
}

// Function to receive a modified file over a network socket
// Parameters:
// - sockfd: The socket descriptor
// - filePath: The path to save the received file
void receiveModifiedFile(SOCKET sockfd, const char filePath[]) {
    char* buffer = malloc(MAX);   // Buffer to store received file contents
    FILE* fp;                      // File pointer
    const char* modifiedFilePath = changeExtension(filePath);  // Modified file path

    // Check for memory allocation error
    if (buffer == NULL) {
        printf("Error allocating memory for buffer.\n");
        return;
    }

    // Attempt to open the file in binary write mode
    if (fopen_s(&fp, modifiedFilePath, "wb") != 0) {
        printf("Error opening file for writing.\n");
        free(buffer);
        return;
    }

    // Receive file contents from client and write to file
    int n;
    (n = recv(sockfd, buffer, MAX, 0));
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

// Function to connect to a server
// Parameters:
// - filePath: The path to the file to compile
// Returns:
// - 1 on success, otherwise 0
int connectToServer(const char* filePath) {
    WSADATA wsaData;              // Structure for Winsock data
    struct sockaddr_in client;    // Structure for client socket address
    
    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 0;
    }

    // Create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed...\n");
        WSACleanup();
        return 0;
    }

    // Initialize client socket address
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    
    // Convert IP address string to binary format
    if(inet_pton(AF_INET, "127.0.0.1", &(client.sin_addr)) <= 0) {
        printf("Invalid address/ Address not supported\n");
        closesocket(sockfd);
        WSACleanup();
        return 0;
    }
    client.sin_port = htons(PORT);  // Set port number

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&client, sizeof(client)) != 0) {
        printf("Connection with the server failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 0;
    } else {
        printf("Connected to the server..\n");
    }

    return 1;
}

// Function to compile a file by sending it to the server and receiving a modified version
// Parameters:
// - filePath: The path to the file to compile
// Returns:
// - 1 on success, otherwise 0
int compile(const char* filePath) {
    // Send the file to the server
    sendFile(sockfd, filePath);

    // Receive the modified file from the server
    receiveModifiedFile(sockfd, filePath);

    return 1;
}

#include "clientWindows.h"

char* changeExtension(char* myStr) {
    char* retStr;
    char* lastExt;
    if (myStr == NULL) return NULL;
    if ((retStr = malloc(strlen(myStr) + 5)) == NULL) return NULL;
    strcpy(retStr, myStr);
    lastExt = strrchr(retStr, '.');
    if (lastExt != NULL)
        *lastExt = '\0';
    strcat(retStr, ".spv");
    return retStr;
}

void sendFile(SOCKET sockfd, const char* filePath) {
    char* buffer = malloc(MAX);
    FILE* fp;

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


    // Close the file
    fclose(fp);
    free(buffer);
}

void receiveModifiedFile(SOCKET sockfd, const char filePath[]) {
    char* buffer = malloc(MAX);
    FILE* fp;
    const char* modifiedFilePath = changeExtension(filePath);

    if (buffer == NULL) {
        printf("Error allocating memory for buffer.\n");
        return;
    }

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

    fclose(fp);
    free(buffer);
}

int connectToServer(const char* filePath) {
    WSADATA wsaData;
    struct sockaddr_in client;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed.\n");
        return 1;
    }

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == INVALID_SOCKET) {
        printf("Socket creation failed...\n");
        WSACleanup();
        return 1;
    }
    printf("Socket successfully created..\n");

    memset(&client, 0, sizeof(client));

    // assign IP, PORT
    client.sin_family = AF_INET;
    if(inet_pton(AF_INET, "127.0.0.1", &(client.sin_addr)) <= 0) {
		printf("Invalid address/ Address not supported\n");
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}
    client.sin_port = htons(PORT);  

    // connect the client socket to server socket
    if (connect(sockfd, (struct sockaddr*)&client, sizeof(client)) != 0) {
        printf("Connection with the server failed: %d\n", WSAGetLastError());
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    else
        printf("Connected to the server..\n");

    // close the socket
    //closesocket(sockfd);
    //WSACleanup();

    return 1;
}

int compile(const char* filePath) {
	sendFile(sockfd, filePath);
	receiveModifiedFile(sockfd, filePath);
	return 1;
}
#include "options.h"
#define _WIN32_WINNT 0x501
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ws2spi.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string>

int tcp::server(void) {
    int bufferlen = 512;
    char buffer[bufferlen];

    const char *bufferreply = "I received your message";

    std::string port;

    std::cout << "Port: ";

    std::getline(std::cin, port);

    // Initialize Winsock
    struct addrinfo hints;
    struct addrinfo *result = NULL;
    WSADATA wsadata;
    int iresult = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (iresult != 0) {
        std::cerr << "WSAStartup failed: " << iresult;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iresult = getaddrinfo(NULL, port.c_str(), &hints, &result);
    if (iresult != 0) {
        std::cerr << "getaddrinfo failed: " << iresult;
        WSACleanup();
        return 1;
    }

    // Create a socket
    SOCKET listensocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listensocket == INVALID_SOCKET) {
        std::cerr << "Error creating a socket: " << WSAGetLastError();
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Bind the socket
    iresult = bind(listensocket, result->ai_addr, (int)result->ai_addrlen);
    if (iresult == SOCKET_ERROR) {
        std::cerr << "Error binding socket: " << WSAGetLastError();
        freeaddrinfo(result);
        closesocket(listensocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    // Listen on the socket for a client
    if (listen(listensocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Error listening to a socket: " << WSAGetLastError();
        closesocket(listensocket);
        WSACleanup();
        return 1;
    }

    // Accept a connection from a client
    SOCKET ClientSocket = INVALID_SOCKET;

    ClientSocket = accept(listensocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        std::cerr << "Error accepting a client socket: " << WSAGetLastError();
        closesocket(listensocket);
        WSACleanup();
        return 1;
    }

    // Close Listen Socket, because it is no longer needed
    closesocket(listensocket);

    std::cout << "Client connected." << std::endl;

    int isendresult;

    // Receive and send data
    do {
        iresult = recv(ClientSocket, buffer, bufferlen, 0);
        if (iresult > 0) {
            std::cout << "Received Message: " << buffer << std::endl;
            std::cout << "Bytes received: " << iresult << std::endl;
            isendresult = send(ClientSocket, bufferreply, (int)strlen(bufferreply)+1, 0);
            if (isendresult == SOCKET_ERROR) {
                std::cerr << "Error sending a message to the client: " << WSAGetLastError();
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            std::cout << "Message sent: " << bufferreply << std::endl;
            std::cout << "Bytes sent: " << (int)strlen(bufferreply)+1 << std::endl;
        } else if (iresult == 0) {
            std::cout << "Connection closed..." << std::endl;
        } else {
            std::cerr << "recv failed: " << WSAGetLastError();
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    } while (iresult > 0);

    // Shutdown the connection for send since no more data will be sent
    iresult = shutdown(ClientSocket, SD_SEND);
    if (iresult == SOCKET_ERROR) {
        std::cerr << "Shutdown failed: " << WSAGetLastError();
        closesocket(ClientSocket);
        WSACleanup();
    }


    // Disconnect
    closesocket(ClientSocket);
    WSACleanup();

    std::cin.get();

    return 0;
}

int tcp::client (void) {
    // Declare server information
    std::string server_address;
    std::string server_port;

    std::cout << "IPv4 Address: ";
    getline(std::cin, server_address);

    std::cout << "Port: ";
    getline(std::cin, server_port);

    struct addrinfo *result;
    struct addrinfo *ptr;
    struct addrinfo hints;

    int bufferlen = 512;
    char buffer[bufferlen];
    std::string sendbuffer;

    // Stores an integer result
    int iresult;

    // Initialize Winsock
    WSADATA wsadata;

    iresult = WSAStartup(MAKEWORD(2,2), &wsadata);
    if (iresult != 0) {
        std::cerr << "Error initializing winsock: " << iresult;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the address and port
    iresult = getaddrinfo(server_address.c_str(), server_port.c_str(), &hints, &result);
    if (iresult != 0) {
        std::cerr << "Error resolving the address and port: " << iresult;
        WSACleanup();
        return 1;
    }

    ptr = result;

    // Create a socket
    SOCKET connectsocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (connectsocket == INVALID_SOCKET) {
        std::cerr << "Error creating a socket: " << WSAGetLastError();
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    iresult = connect(connectsocket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iresult == SOCKET_ERROR) {
        std::cerr << "Error establishing a connection: " << WSAGetLastError();
        closesocket(connectsocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    if (connectsocket == INVALID_SOCKET) {
        std::cerr << "Unable to connect to server!";
        closesocket(connectsocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Send Message: ";
    getline(std::cin, sendbuffer);

    // Send and receive data
    iresult = send(connectsocket, sendbuffer.c_str(), sendbuffer.length()+1, 0);
    if (iresult == SOCKET_ERROR) {
        std::cerr << "Send failed: " << WSAGetLastError();
        closesocket(connectsocket);
        WSACleanup();
    }

    std::cout << "Bytes sent: " << iresult << std::endl;

    // Shutdown the connection for sending since no more data will be sent
    iresult = shutdown(connectsocket, SD_SEND);
    if (iresult == SOCKET_ERROR) {
        std::cerr << "Shutdown failed: " << WSAGetLastError();
        closesocket(connectsocket);
        WSACleanup();
        return 1;
    }

    // Receive data until the server closes the connection
    do {
        iresult = recv(connectsocket, buffer, bufferlen, 0);
        if (iresult > 0) {
            std::cout << "Server reply: " << buffer << std::endl;
            std::cout << "Bytes received: " << iresult << std::endl;
        } else if (iresult == 0) {
            std::cout << "Connection closed..." << std::endl;
        } else {
            std::cerr << "recv failed: " << WSAGetLastError();
        }
    } while (iresult > 0);

    // Disconnect
    closesocket(connectsocket);
    WSACleanup();

    std::cin.get();
    return 0;
}

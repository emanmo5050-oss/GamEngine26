#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 5555
#define MAX_CLIENTS 4

vector<SOCKET> clients;
mutex clientMutex;

void broadcastMessage(const string& msg, SOCKET sender) {
    lock_guard<mutex> lock(clientMutex);
    for (SOCKET client : clients) {
        if (client != sender)
            send(client, msg.c_str(), msg.size(), 0);
    }
}

void handleClient(SOCKET clientSocket) {
    char buffer[1024];
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
    cout << "A client disconnected.\n";
    lock_guard<mutex> lock(clientMutex);
    for (auto it = clients.begin(); it != clients.end(); ++it){
        if (*it == clientSocket) {
            clients.erase(it);
            break;
        }
    }
    closesocket(clientSocket);
    break;
}
        string msg(buffer, bytesReceived);
        cout << "Received: " << msg << "\n";
        broadcastMessage(msg, clientSocket);
    }
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));

    listen(serverSocket, MAX_CLIENTS);
    cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) continue;

        cout << "New client connected!\n";
        {
            lock_guard<mutex> lock(clientMutex);
            clients.push_back(clientSocket);
        }
        thread(handleClient, clientSocket).detach();
    }

    WSACleanup();
    return 0;
}
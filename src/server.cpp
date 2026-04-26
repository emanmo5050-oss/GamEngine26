#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <vector>
#include <mutex>
#include <string>
#include <sstream>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 5555
#define MAX_CLIENTS 2

// ─── Game State ───────────────────────────────────────────────────
struct Player {
    SOCKET socket;
    int id;
    string role;  // "TOP" or "BOTTOM"
    int x, y;
    bool connected;
};

int score = 0;
Player players[2];
int playerCount = 0;
mutex clientMutex;

// ─── Send to one client ───────────────────────────────────────────
void sendToClient(SOCKET sock, const string& msg) {
    send(sock, msg.c_str(), msg.size(), 0);
}

// ─── Broadcast to all clients ─────────────────────────────────────
void broadcast(const string& msg) {
    lock_guard<mutex> lock(clientMutex);
    for (int i = 0; i < 2; i++) {
        if (players[i].connected)
            sendToClient(players[i].socket, msg);
    }
}

// ─── Parse and handle incoming packets ───────────────────────────
void handlePacket(const string& packet, int playerID) {
    // Packet format: PlayerID:Action:X:Y
    // Example: "1:SLIDE:5:0" or "2:JUMP:3:1"
    stringstream ss(packet);
    string idStr, action, xStr, yStr;

    getline(ss, idStr, ':');
    getline(ss, action, ':');
    getline(ss, xStr, ':');
    getline(ss, yStr, ':');

    int x = xStr.empty() ? 0 : stoi(xStr);
    int y = yStr.empty() ? 0 : stoi(yStr);

    if (action == "SLIDE") {
        players[playerID - 1].x = x;
        broadcast("STATE:" + to_string(playerID) + ":SLIDE:" + to_string(x) + ":" + to_string(y));
    }
    else if (action == "SHOOT") {
        broadcast("STATE:" + to_string(playerID) + ":SHOOT:" + to_string(x) + ":" + to_string(y));
    }
    else if (action == "JUMP") {
        players[playerID - 1].y = y;
        broadcast("STATE:" + to_string(playerID) + ":JUMP:" + to_string(x) + ":" + to_string(y));
    }
    else if (action == "SCORED") {
        // A point was scored (box destroyed or jumped)
        score++;
        broadcast("SCORE:" + to_string(score));
        cout << "Score updated: " << score << "\n";
    }
}

// ─── Handle each client on its own thread ────────────────────────
void handleClient(int playerIndex) {
    SOCKET sock = players[playerIndex].socket;
    int playerID = players[playerIndex].id;
    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cout << "Player " << playerID << " (" << players[playerIndex].role << ") disconnected.\n";
            players[playerIndex].connected = false;
            broadcast("SERVER:PLAYER_LEFT:" + to_string(playerID));
            closesocket(sock);
            break;
        }
        string packet(buffer, bytesReceived);
        cout << "[P" << playerID << "] " << packet << "\n";
        handlePacket(packet, playerID);
    }
}

// ─── Main ─────────────────────────────────────────────────────────
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
    cout << "Waiting for 2 players...\n";

    // Accept exactly 2 players and assign roles
    while (playerCount < 2) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) continue;

        lock_guard<mutex> lock(clientMutex);

        players[playerCount].socket = clientSocket;
        players[playerCount].id = playerCount + 1;
        players[playerCount].connected = true;
        players[playerCount].x = 0;
        players[playerCount].y = 0;

        // Assign role based on connection order
        if (playerCount == 0) {
            players[playerCount].role = "TOP";
            sendToClient(clientSocket, "ROLE:1:TOP");
            cout << "Player 1 connected → Role: TOP (Triangle)\n";
        } else {
            players[playerCount].role = "BOTTOM";
            sendToClient(clientSocket, "ROLE:2:BOTTOM");
            cout << "Player 2 connected → Role: BOTTOM (Ground Runner)\n";
        }

        playerCount++;

        // Start game when both players are connected
        if (playerCount == 2) {
            broadcast("SERVER:START");
            cout << "Both players connected! Game starting...\n";
        }
    }

    // Start a thread for each player
    thread t1(handleClient, 0);
    thread t2(handleClient, 1);

    t1.join();
    t2.join();

    WSACleanup();
    return 0;
}
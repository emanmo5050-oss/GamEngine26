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
    string role;      // "TOP" or "BOTTOM"
    int x, y;
    bool alive;       // NEW: tracks whether this player is alive
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

// ─── Task 2: serializeAll ─────────────────────────────────────────
// Packages the full world state into one string for broadcasting.
// Format: WORLDSTATE:score|P1:id:role:x:y:alive|P2:id:role:x:y:alive
//
// Example: "WORLDSTATE:3|P1:1:TOP:5:0:1|P2:2:BOTTOM:3:1:1"
//
// The server calls this after every state-changing event so both
// clients stay in sync from a single authoritative snapshot.
string serializeAll() {
    string state = "WORLDSTATE:" + to_string(score);
    for (int i = 0; i < 2; i++) {
        state += "|P" + to_string(i + 1) + ":"
               + to_string(players[i].id)    + ":"
               + players[i].role             + ":"
               + to_string(players[i].x)    + ":"
               + to_string(players[i].y)    + ":"
               + (players[i].alive ? "1" : "0");
    }
    return state;
}

// ─── Broadcast the full world state snapshot ─────────────────────
// Called after every action so clients never rely on guessing state.
void broadcastWorldState() {
    string snapshot = serializeAll();
    // Bypass the normal broadcast lock since callers may already hold it
    for (int i = 0; i < 2; i++) {
        if (players[i].connected)
            sendToClient(players[i].socket, snapshot);
    }
}

// ─── Parse and handle incoming packets ───────────────────────────
void handlePacket(const string& packet, int playerID) {
    // Packet format: PlayerID:Action:X:Y
    stringstream ss(packet);
    string idStr, action, xStr, yStr;

    getline(ss, idStr, ':');
    getline(ss, action, ':');
    getline(ss, xStr, ':');
    getline(ss, yStr, ':');

    int x = xStr.empty() ? 0 : stoi(xStr);
    int y = yStr.empty() ? 0 : stoi(yStr);

    {
        lock_guard<mutex> lock(clientMutex);

        if (action == "SLIDE") {
            players[playerID - 1].x = x;
        }
        else if (action == "JUMP") {
            players[playerID - 1].y = y;
        }
        else if (action == "SHOOT") {
            // Position unchanged; broadcast will carry current coords
        }
        // ── Task 2: Server-authoritative SCORED ──────────────────────────
        // The client requests a score, but the server decides whether to
        // grant it and what the new score is.  This prevents both clients
        // from independently incrementing their local counters and drifting.
        else if (action == "SCORED") {
            score++;
            cout << "[AUTH] Score updated by P" << playerID << " → " << score << "\n";
        }
        // ── Task 2: Server-authoritative DEATH ───────────────────────────
        // Clients send DEATH when they detect a kill, but only the server
        // actually marks the player dead and increments the score.
        // Format: PlayerID:DEATH:victimID:0
        else if (action == "DEATH") {
            int victimID = x;  // x field carries the victim's player ID
            if (victimID >= 1 && victimID <= 2) {
                players[victimID - 1].alive = false;
                score++;
                cout << "[AUTH] Player " << victimID << " died. Score → " << score << "\n";
            }
        }

        // After every state change, push the authoritative full snapshot
        broadcastWorldState();
    }

    // Also send the specific action event so clients can trigger animations
    if (action == "SLIDE" || action == "SHOOT" || action == "JUMP") {
        broadcast("STATE:" + to_string(playerID) + ":" + action + ":"
                  + to_string(x) + ":" + to_string(y));
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
            {
                lock_guard<mutex> lock(clientMutex);
                players[playerIndex].connected = false;
            }
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

        players[playerCount].socket    = clientSocket;
        players[playerCount].id        = playerCount + 1;
        players[playerCount].connected = true;
        players[playerCount].alive     = true;   // NEW
        players[playerCount].x         = 0;
        players[playerCount].y         = 0;

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

        if (playerCount == 2) {
            // Broadcast start signal then immediately push the initial world state
            for (int i = 0; i < 2; i++)
                sendToClient(players[i].socket, "SERVER:START");
            broadcastWorldState();
            cout << "Both players connected! Game starting...\n";
        }
    }

    thread t1(handleClient, 0);
    thread t2(handleClient, 1);

    t1.join();
    t2.join();

    WSACleanup();
    return 0;
}

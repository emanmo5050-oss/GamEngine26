#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>
#include <atomic>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

#define PORT 5555
#define SERVER_IP "127.0.0.1"  // Change to server's IP for LAN

atomic<bool> running(true);
int myPlayerID = 0;
string myRole = "";
int score = 0;

// ─── Handle incoming messages from server ────────────────────────
void receiveMessages(SOCKET sock) {
    char buffer[1024];
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            cout << "\n[Disconnected from server.]\n";
            running = false;
            break;
        }

        string msg(buffer, bytesReceived);

        // Role assignment from server
        if (msg.substr(0, 5) == "ROLE:") {
            // Format: ROLE:PlayerID:ROLE
            myPlayerID = msg[5] - '0';
            myRole = msg.substr(7);
            cout << "\n You are Player " << myPlayerID << " | Role: " << myRole << "\n";
            if (myRole == "TOP")
                cout << "Controls: A/D to slide | F to shoot\n";
            else
                cout << "Controls: SPACE to jump\n";
        }
        // Game start signal
        else if (msg == "SERVER:START") {
            cout << "\n Both players connected! Game is starting...\n> ";
        }
        // Score update
        else if (msg.substr(0, 6) == "SCORE:") {
            score = stoi(msg.substr(6));
            cout << "\n Score: " << score << "\n> ";
        }
        // Another player left
        else if (msg.substr(0, 19) == "SERVER:PLAYER_LEFT:") {
            cout << "\n Other player disconnected.\n";
            running = false;
        }
        // State updates (movement, actions)
        else if (msg.substr(0, 6) == "STATE:") {
            cout << "\n[UPDATE] " << msg << "\n> ";
        }

        cout.flush();
    }
}

// ─── Send packet to server ────────────────────────────────────────
void sendPacket(SOCKET sock, const string& action, int x, int y) {
    // Format: PlayerID:Action:X:Y
    string packet = to_string(myPlayerID) + ":" + action + ":" + to_string(x) + ":" + to_string(y);
    send(sock, packet.c_str(), packet.size(), 0);
}

// ─── Main ─────────────────────────────────────────────────────────
int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        cerr << "Could not connect to server.\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    cout << "Connected to server at " << SERVER_IP << ":" << PORT << "\n";
    cout << "Waiting for role assignment...\n";

    // Start receive thread
    thread recvThread(receiveMessages, sock);

    // Input loop
    while (running) {
        cout << "> ";
        string input;
        getline(cin, input);

        if (input == "QUIT") {
            running = false;
            break;
        }
        if (input.empty()) continue;

        // Player 1 (TOP) controls
        if (myRole == "TOP") {
            if (input == "A")       sendPacket(sock, "SLIDE", -1, 0);
            else if (input == "D")  sendPacket(sock, "SLIDE", 1, 0);
            else if (input == "F")  sendPacket(sock, "SHOOT", 0, 0);
        }
        // Player 2 (BOTTOM) controls
        else if (myRole == "BOTTOM") {
            if (input == "SPACE")   sendPacket(sock, "JUMP", 0, 1);
        }

        // Score a point
        if (input == "SCORED")      sendPacket(sock, "SCORED", 0, 0);
    }

    closesocket(sock);
    WSACleanup();
    if (recvThread.joinable())
        recvThread.join();

    cout << "Disconnected.\n";
    return 0;
}
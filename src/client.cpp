#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <string>
#include <atomic>
#pragma comment(lib, "ws2_32.lib")
using namespace std;

#define PORT 5555
#define SERVER_IP "127.0.0.1"  // Change to server's IP for LAN play

atomic<bool> running(true);

// ─── Receive Thread ───────────────────────────────────────────────
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
        cout << "\n[Player] " << msg << "\n> ";
        cout.flush();
    }
}

// ─── Main (Connect + Send) ────────────────────────────────────────
int main() {
    // 1. Init Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed.\n";
        return 1;
    }

    // 2. Create socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        cerr << "Socket creation failed.\n";
        WSACleanup();
        return 1;
    }

    // 3. Connect to server
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

    // 4. Ask for username
    string username;
    cout << "Enter your username: ";
    getline(cin, username);

    // Announce join to other players
    string joinMsg = username + " has joined the game!";
    send(sock, joinMsg.c_str(), joinMsg.size(), 0);

    // 5. Start receive thread
    thread recvThread(receiveMessages, sock);

    // 6. Send loop (game input)
    cout << "Commands: MOVE <x> <y> | ATTACK | CHAT <message> | QUIT\n";
    while (running) {
        cout << "> ";
        string input;
        getline(cin, input);

        if (input == "QUIT") {
            running = false;
            break;
        }

        if (input.empty()) continue;

        // Format: "username:command"
        string packet = username + ":" + input;
        send(sock, packet.c_str(), packet.size(), 0);
    }

    // 7. Cleanup
    closesocket(sock);
    WSACleanup();
    if (recvThread.joinable())
        recvThread.join();

    cout << "Disconnected.\n";
    return 0;
}
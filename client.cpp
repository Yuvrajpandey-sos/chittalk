#include <iostream>
#include <thread>
#include <cstring>
#include <string>
#include <winsock2.h>
#include <functional>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void receiveMessages(SOCKET sock) {
    char buffer[1024];

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(sock, buffer, sizeof(buffer), 0);

        if (bytes > 0) {
            cout << "\nMessage: " << buffer << endl;
        }
    }
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

    std::thread t(receiveMessages, sock);
    t.detach();

    while (true) {
        string msg;
        getline(cin, msg);
        send(sock, msg.c_str(), msg.size(), 0);
    }

    closesocket(sock);
    WSACleanup();
}
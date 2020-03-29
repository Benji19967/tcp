#include <iostream>
#include <winsock2.h>

#define NI_MAXHOST (1024)
#define NI_MAXSERV (1024)

using namespace std;

int main() {

    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);

    if (wsOk != 0)
    {
        cerr << "Can't initialize winsock! Quitting" << endl;
        return 0;
    }

    // Create a socket (ref number to access "this" port on an ip address)
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return 0;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell winsock the socket is ready for listening
    listen(listening, SOMAXCONN);

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    // Should check for invalid socket

    char host[NI_MAXHOST]; // Client's remote name
    char service[NI_MAXSERV]; // Service (i.e. port) the client is connected on

    ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST); on UNIX systems
    ZeroMemory(service, NI_MAXSERV);
    gethostname(host, sizeof(host));

    if (gethostname(host, sizeof(host)) == 0)
    {
        cout << host << " connected on port " << service << endl;
    }
    else
    {
        char* IP = inet_ntoa(client.sin_addr);
        strcpy(host, IP);
        cout << host << " connected on port" << ntohs(client.sin_port) << endl;
    }

    // Close listening socket
    closesocket(listening);

    // While loop: accept and echo message back to client
    char buf[4096];

    while (true)
    {
        ZeroMemory(buf, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << "Error in recv(). Quitting" << endl;
        }

        if (bytesReceived == 0)
        {
            cout << "Client disconnected" << endl;
            break;
        }

        cout << string(buf, 0, bytesReceived) << endl;

        // Echo message back to client
        send(clientSocket, buf, bytesReceived + 1, 0); // +1: ZeroMemory adds null terminator
    }

    // Close socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();
}
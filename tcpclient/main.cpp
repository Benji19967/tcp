#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

int main()
{
    string ipAddress = "127.0.0.1"; // IP Address of tcp server
    int port = 54000; // Listening port # on the tcp server

    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Can't start Winsock, Err #" << wsResult << endl;
        return 99;
    }

    // Create socket (Communicates with Network layer)
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET)
    {
        cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
        WSACleanup();
        return 99;
    }

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port); // Network is big endian, some machine are little endian - htons takes care of this
    hint.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        WSACleanup();
        return 99;
    }

    // Do-while loop ro send and receive data
    char buf[4096];
    string userInput;

    do
    {
        // Prompt the user for some text
        cout << "> ";
        getline(cin, userInput);

        if (userInput.size() > 0) // Make sure user typed in something
        {
            // Send the text (Converting to c-style string adds trailing 0 character - use + 1 in size)
            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
            if (sendResult != SOCKET_ERROR)
            {
                // Wait for the response
                ZeroMemory(buf, 4096);
                int bytesReceived = recv(sock, buf, 4096, 0);
                if (bytesReceived > 0)
                {
                    // Echo response to console
                    cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
                }
            }
        }
    } while (userInput.size() > 0);

    // Close down
    closesocket(sock);
    WSACleanup();
    return 0;
}

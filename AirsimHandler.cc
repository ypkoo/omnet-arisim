//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "AirsimHandler.h"



Define_Module(AirsimHandler);

void error(const char *msg)
{
    perror(msg);
    exit(1);
}


void AirsimHandler::initialize()
{
    std::cout << "AirsimHandler initializing..." << std::endl;
    WSADATA wsaData;
        // int iResult;

        SOCKET ListenSocket = INVALID_SOCKET;
    //  SOCKET ClientSocket = INVALID_SOCKET;
    //  ClientSocket = INVALID_SOCKET;
    //  SOCKET ClientSocket;

        struct addrinfo *result = NULL;
        struct addrinfo hints;

        int iSendResult;
        // char recvbuf[DEFAULT_BUFLEN];
        // int recvbuflen = DEFAULT_BUFLEN;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (iResult != 0) {
            printf("WSAStartup failed with error: %d\n", iResult);
            // return 1;
        }

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
        if (iResult != 0) {
            printf("getaddrinfo failed with error: %d\n", iResult);
            WSACleanup();
            // return 1;
        }

        // Create a SOCKET for connecting to server
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            // return 1;
        }

        // Setup the TCP listening socket
        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("bind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            // return 1;
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            printf("listen failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
    //      return 1;
        }

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
    //      return 1;
        }

        printf("client accepted.\n");
        // No longer need server socket
        closesocket(ListenSocket);

        unsigned long l = 1;
        ioctlsocket(ClientSocket, FIONBIO, &l);

        cMessage * msg = new cMessage("socket accept done.");
        scheduleAt(0, msg);
}

void AirsimHandler::handleMessage(cMessage *msg)
{

    iResult = recv(ClientSocket, recvbuf, DEFAULT_BUFLEN, 0);
    if (iResult > 0) {
        // printf("Bytes received: %d\n", iResult);
        // printf("Received msg: %s\n", recvbuf);
        char msg[DEFAULT_BUFLEN];
        memset(&msg, 0, sizeof(msg));
        strncpy(msg, recvbuf, iResult);

        std::cout << msg << std::endl;
        std::cout << "HI!" << std::endl;


        // Json::CharReaderBuilder builder;
        // Json::CharReader * reader = builder.newCharReader();

        // Json::Value root;
        // string errors;

        // string text = string(recvbuf);

        // bool parsingSuccessful = reader->parse(text.c_str(), text.c_str() + text.size(), &root, &errors);
        // if (!parsingSuccessful) {
        //  printf("error parsing json\n");
        // }

        // printf("Received msg: %s\n", root);
        // for (Json::Value::const_iterator outer = root.begin(); outer != root.end(); outer++)
        // {
        //  for (Json::Value::const_iterator inner = (*outer).begin(); inner != (*outer).end(); inner++)
        //  {
        //      cout << inner.key() << ": " << *inner << endl;
        //  }
        // }



        // // Echo the buffer back to the sender
        // iSendResult = send(ClientSocket, recvbuf, iResult, 0);
        // if (iSendResult == SOCKET_ERROR) {
        //  printf("send failed with error: %d\n", WSAGetLastError());
        //  closesocket(ClientSocket);
        //  WSACleanup();
        //  return 1;
        // }
        // printf("Bytes sent: %d\n", iSendResult);
    }
    scheduleAt(simTime() + 1, msg);
}

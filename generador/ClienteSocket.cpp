#include "ClienteSocket.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <sstream>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

ClienteSocket::ClienteSocket(const std::string& ip, int puerto)
    : ip_(ip), puerto_(puerto), listo_(false) {
    WSADATA wsaData;
    listo_ = (WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);
}

ClienteSocket::~ClienteSocket() {
    if (listo_) WSACleanup();
}

bool ClienteSocket::enviar(const std::string& mensaje, std::string& respuesta) {
    if (!listo_) {
        respuesta = "no se pudo inicializar Winsock";
        return false;
    }

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        respuesta = "fallo al crear el socket";
        return false;
    }

    struct sockaddr_in servidor;
    memset(&servidor, 0, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons((unsigned short)puerto_);
    servidor.sin_addr.s_addr = inet_addr(ip_.c_str());
    if (servidor.sin_addr.s_addr == INADDR_NONE) {
        closesocket(s);
        respuesta = "direccion IP invalida";
        return false;
    }

    if (connect(s, (struct sockaddr*)&servidor, sizeof(servidor)) == SOCKET_ERROR) {
        std::ostringstream ss;
        ss << "fallo en connect (codigo " << WSAGetLastError() << ")";
        respuesta = ss.str();
        closesocket(s);
        return false;
    }

    send(s, mensaje.c_str(), (int)mensaje.size(), 0);

    char buffer[1024];
    int n = recv(s, buffer, sizeof(buffer) - 1, 0);
    closesocket(s);

    if (n <= 0) {
        respuesta = "el servidor no respondio";
        return false;
    }
    buffer[n] = '\0';
    respuesta = std::string(buffer);
    return true;
}

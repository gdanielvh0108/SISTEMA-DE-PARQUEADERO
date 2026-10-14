#include "ServidorSocket.h"
#include <cstring>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

ServidorSocket::ServidorSocket()
    : servidor_((size_t)INVALID_SOCKET), cliente_((size_t)INVALID_SOCKET), activo_(false) {}

ServidorSocket::~ServidorSocket() {
    detener();
}

bool ServidorSocket::activo() const { return activo_; }

bool ServidorSocket::iniciar(int puerto) {
    if (activo_) return true;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) return false;

    SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    struct sockaddr_in direccion;
    memset(&direccion, 0, sizeof(direccion));
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons((unsigned short)puerto);

    if (bind(s, (struct sockaddr*)&direccion, sizeof(direccion)) == SOCKET_ERROR ||
        listen(s, 5) == SOCKET_ERROR) {
        closesocket(s);
        WSACleanup();
        return false;
    }

    servidor_ = (size_t)s;
    activo_ = true;
    return true;
}

// Espera (con tiempo máximo) a que el socket tenga algo para leer
static bool esperarLectura(SOCKET s, int timeoutMs) {
    fd_set lectura;
    FD_ZERO(&lectura);
    FD_SET(s, &lectura);
    struct timeval tv;
    tv.tv_sec = timeoutMs / 1000;
    tv.tv_usec = (timeoutMs % 1000) * 1000;
    return select((int)s + 1, &lectura, NULL, NULL, &tv) > 0;
}

std::string ServidorSocket::recibir(int timeoutMs) {
    if (!activo_) return "";

    // Cierra una conexión anterior que no se hubiera respondido
    if (cliente_ != (size_t)INVALID_SOCKET) {
        closesocket((SOCKET)cliente_);
        cliente_ = (size_t)INVALID_SOCKET;
    }

    SOCKET srv = (SOCKET)servidor_;
    if (!esperarLectura(srv, timeoutMs)) return "";

    SOCKET c = accept(srv, NULL, NULL);
    if (c == INVALID_SOCKET) return "";

    // Si el generador no envía nada en 5 s, no nos quedamos colgados
    char buffer[1024];
    int n = 0;
    if (esperarLectura(c, 5000)) {
        n = recv(c, buffer, sizeof(buffer) - 1, 0);
    }
    if (n <= 0) {
        closesocket(c);
        return "";
    }

    buffer[n] = '\0';
    cliente_ = (size_t)c;
    return std::string(buffer);
}

void ServidorSocket::responder(const std::string& texto) {
    if (cliente_ == (size_t)INVALID_SOCKET) return;
    send((SOCKET)cliente_, texto.c_str(), (int)texto.size(), 0);
    closesocket((SOCKET)cliente_);
    cliente_ = (size_t)INVALID_SOCKET;
}

void ServidorSocket::detener() {
    if (!activo_) return;
    if (cliente_ != (size_t)INVALID_SOCKET) {
        closesocket((SOCKET)cliente_);
        cliente_ = (size_t)INVALID_SOCKET;
    }
    closesocket((SOCKET)servidor_);
    WSACleanup();
    servidor_ = (size_t)INVALID_SOCKET;
    activo_ = false;
}

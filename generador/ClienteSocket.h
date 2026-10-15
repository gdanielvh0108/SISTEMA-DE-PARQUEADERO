// Cliente TCP (Winsock): envía un mensaje al servidor y espera la respuesta.
#ifndef CLIENTESOCKET_H
#define CLIENTESOCKET_H

#include <string>

class ClienteSocket {
public:
    ClienteSocket(const std::string& ip, int puerto);
    ~ClienteSocket();

    // Abre una conexión, envía el mensaje y guarda la respuesta (o el error).
    bool enviar(const std::string& mensaje, std::string& respuesta);

private:
    std::string ip_;
    int puerto_;
    bool listo_;  // Winsock inicializado
};

#endif

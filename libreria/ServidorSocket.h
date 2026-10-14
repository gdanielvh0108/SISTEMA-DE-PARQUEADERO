// Servidor TCP (Winsock): espera una conexión, lee un mensaje y responde.
#ifndef SERVIDORSOCKET_H
#define SERVIDORSOCKET_H

#include <string>
#include <cstddef>

class ServidorSocket {
public:
    ServidorSocket();
    ~ServidorSocket();

    bool iniciar(int puerto);

    // Espera hasta timeoutMs por un mensaje. Devuelve "" si no llegó nada.
    // Si devuelve un mensaje, hay que llamar a responder().
    std::string recibir(int timeoutMs);
    void responder(const std::string& texto);

    void detener();
    bool activo() const;

private:
    size_t servidor_;  // SOCKET que escucha (guardado como entero)
    size_t cliente_;   // SOCKET de la conexión en curso
    bool activo_;
};

#endif

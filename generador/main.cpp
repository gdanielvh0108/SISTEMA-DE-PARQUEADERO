// Generador de placas (cliente). Archivo principal.
// Envía una placa cada 2 o 5 segundos al servidor.
//
// Compilar: g++ main.cpp GeneradorPlacas.cpp ClienteSocket.cpp -o generador.exe -lws2_32

#include <iostream>
#include <string>
#include <cstdlib>
#include <windows.h>

#include "GeneradorPlacas.h"
#include "ClienteSocket.h"

#define PUERTO 8080
#define TOTAL_CELDAS 20   // debe coincidir con el visualizador

// Lee una línea; devuelve "" si no hay más entrada
static std::string leerLinea(const std::string& pregunta) {
    std::cout << pregunta;
    std::string linea;
    if (!std::getline(std::cin, linea)) return "";
    return linea;
}

int main() {
    std::string ip = leerLinea("Ingrese IP del servidor (Enter = 127.0.0.1): ");
    if (ip.empty()) ip = "127.0.0.1";

    int intervalo = 0;
    while (intervalo != 2 && intervalo != 5) {
        std::string t = leerLinea("Intervalo de envio en segundos (2 o 5): ");
        if (t.empty() && !std::cin) { intervalo = 2; break; }
        intervalo = atoi(t.c_str());
    }

    GeneradorPlacas generador(TOTAL_CELDAS);
    ClienteSocket cliente(ip, PUERTO);

    std::cout << "Enviando a " << ip << ":" << PUERTO << " cada " << intervalo
              << " s. (Ctrl+C para salir)" << std::endl;

    while (true) {
        std::string mensaje = generador.siguienteMensaje();
        std::string respuesta;

        if (cliente.enviar(mensaje, respuesta)) {
            std::cout << "Enviado: " << mensaje << "  ->  " << respuesta << std::endl;
        } else {
            std::cout << "No se pudo enviar " << mensaje << ": " << respuesta << std::endl;
        }

        Sleep(intervalo * 1000);
    }

    return 0;
}

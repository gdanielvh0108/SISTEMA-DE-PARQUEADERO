// Clase principal de la librería dinámica: recibe las placas por socket,
// actualiza el parqueadero y guarda el historial. Es la que usa Python (SWIG).
#ifndef RECEPTOR_H
#define RECEPTOR_H

#include <string>
#include "Parqueadero.h"
#include "ServidorSocket.h"
#include "Historial.h"

class Receptor {
public:
    Receptor(int totalCeldas, const std::string& archivoCsv);

    bool iniciar(int puerto);
    void detener();

    // Espera hasta timeoutMs por un mensaje "PLACA;HORA;CELDA".
    // Devuelve una línea descriptiva del evento, o "" si no llegó nada.
    std::string atender(int timeoutMs);

    // Consultas para la interfaz gráfica (las celdas se numeran desde 1)
    int totalCeldas() const;
    int ocupadas() const;
    int libres() const;
    bool estaOcupada(int celda) const;
    std::string placaEn(int celda) const;
    std::string horaEn(int celda) const;

private:
    Parqueadero parqueadero_;
    ServidorSocket servidor_;
    Historial historial_;
};

#endif

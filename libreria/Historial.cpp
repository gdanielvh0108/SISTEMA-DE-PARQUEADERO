#include "Historial.h"
#include <fstream>
#include <ctime>
#include <algorithm>

static std::string ahora() {
    time_t t = time(NULL);
    struct tm* info = localtime(&t);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", info);
    return std::string(buf);
}

Historial::Historial(const std::string& archivo) : archivo_(archivo) {}

void Historial::guardar(const std::string& placa, const std::string& horaGenerador,
                        int celda, const std::string& resultado, int ocupadas) {
    std::ifstream existe(archivo_.c_str());
    bool nuevo = !existe.good();
    existe.close();

    std::string limpio = resultado;
    std::replace(limpio.begin(), limpio.end(), ';', ' ');

    std::ofstream f(archivo_.c_str(), std::ios::app);
    if (nuevo) {
        f << "fecha_servidor;placa;hora_generador;celda;resultado;ocupadas\n";
    }
    f << ahora() << ";" << placa << ";" << horaGenerador << ";" << celda << ";"
      << limpio << ";" << ocupadas << "\n";
}

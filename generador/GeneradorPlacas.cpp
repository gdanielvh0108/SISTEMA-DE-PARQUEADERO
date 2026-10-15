#include "GeneradorPlacas.h"
#include <cstdlib>
#include <ctime>
#include <sstream>

GeneradorPlacas::GeneradorPlacas(int totalCeldas) {
    srand((unsigned int)time(NULL));
    for (int i = 1; i <= totalCeldas; i++) {
        libres_.push_back(i);
    }
}

std::string GeneradorPlacas::nuevaPlaca() const {
    std::string placa;
    for (int i = 0; i < 3; i++) placa += (char)('A' + rand() % 26);
    for (int i = 0; i < 3; i++) placa += (char)('0' + rand() % 10);
    return placa;
}

std::string GeneradorPlacas::horaActual() const {
    time_t t = time(NULL);
    struct tm* info = localtime(&t);
    char buf[16];
    strftime(buf, sizeof(buf), "%H:%M:%S", info);
    return std::string(buf);
}

std::string GeneradorPlacas::siguienteMensaje() {
    bool repetir;
    if (ocupadas_.empty()) {
        repetir = false;                 // no hay nadie que pueda salir
    } else if (libres_.empty()) {
        repetir = true;                  // parqueadero lleno: tiene que salir alguien
    } else {
        repetir = (rand() % 100) < 40;   // 40% de probabilidad de salida
    }

    std::string placa;
    int celda;

    if (repetir) {
        // Se repite una placa que ya está adentro: libera su celda
        int salto = rand() % (int)ocupadas_.size();
        std::map<std::string, int>::iterator it = ocupadas_.begin();
        for (int i = 0; i < salto; i++) ++it;
        placa = it->first;
        celda = it->second;
        ocupadas_.erase(it);
        libres_.push_back(celda);
    } else {
        // Placa nueva en una celda libre al azar
        do {
            placa = nuevaPlaca();
        } while (ocupadas_.count(placa) > 0);
        int pos = rand() % (int)libres_.size();
        celda = libres_[pos];
        libres_.erase(libres_.begin() + pos);
        ocupadas_[placa] = celda;
    }

    std::ostringstream ss;
    ss << placa << ";" << horaActual() << ";" << celda;
    return ss.str();
}

#include "Parqueadero.h"
#include <sstream>

static std::string aTexto(int n) {
    std::ostringstream ss;
    ss << n;
    return ss.str();
}

Parqueadero::Parqueadero(int totalCeldas) {
    for (int i = 1; i <= totalCeldas; i++) {
        celdas_.push_back(Celda(i));
    }
}

bool Parqueadero::celdaValida(int numeroCelda) const {
    return numeroCelda >= 1 && numeroCelda <= (int)celdas_.size();
}

std::string Parqueadero::registrar(const std::string& placa, const std::string& hora, int numeroCelda) {
    std::map<std::string, int>::iterator it = placaACelda_.find(placa);

    // Placa repetida: se libera la celda que ocupaba
    if (it != placaACelda_.end()) {
        int numero = it->second;
        celdas_[numero - 1].liberar();
        placaACelda_.erase(it);
        return "LIBERADA;" + aTexto(numero);
    }

    // Placa nueva: se ocupa la celda indicada
    if (!celdaValida(numeroCelda)) {
        return "ERROR;celda fuera de rango";
    }
    if (celdas_[numeroCelda - 1].ocupada()) {
        return "ERROR;celda ocupada por otra placa";
    }
    celdas_[numeroCelda - 1].ocupar(placa, hora);
    placaACelda_[placa] = numeroCelda;
    return "OCUPADA;" + aTexto(numeroCelda);
}

int Parqueadero::totalCeldas() const { return (int)celdas_.size(); }
int Parqueadero::ocupadas() const { return (int)placaACelda_.size(); }
int Parqueadero::libres() const { return totalCeldas() - ocupadas(); }

bool Parqueadero::estaOcupada(int numeroCelda) const {
    return celdaValida(numeroCelda) && celdas_[numeroCelda - 1].ocupada();
}

std::string Parqueadero::placaEn(int numeroCelda) const {
    return celdaValida(numeroCelda) ? celdas_[numeroCelda - 1].placa() : "";
}

std::string Parqueadero::horaEn(int numeroCelda) const {
    return celdaValida(numeroCelda) ? celdas_[numeroCelda - 1].hora() : "";
}

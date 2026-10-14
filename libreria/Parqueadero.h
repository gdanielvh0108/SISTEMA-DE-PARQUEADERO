// Estado del parqueadero: un conjunto de celdas y la regla de negocio:
//  - placa nueva  -> ocupa la celda indicada
//  - placa repetida -> libera la celda que tenía
#ifndef PARQUEADERO_H
#define PARQUEADERO_H

#include <string>
#include <vector>
#include <map>
#include "Celda.h"

class Parqueadero {
public:
    explicit Parqueadero(int totalCeldas);

    // Devuelve "OCUPADA;N", "LIBERADA;N" o "ERROR;motivo"
    std::string registrar(const std::string& placa, const std::string& hora, int numeroCelda);

    int totalCeldas() const;
    int ocupadas() const;
    int libres() const;
    bool estaOcupada(int numeroCelda) const;
    std::string placaEn(int numeroCelda) const;
    std::string horaEn(int numeroCelda) const;

private:
    bool celdaValida(int numeroCelda) const;

    std::vector<Celda> celdas_;
    std::map<std::string, int> placaACelda_;  // placa -> número de celda
};

#endif

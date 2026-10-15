// Genera los mensajes "PLACA;HORA;CELDA".
// Una placa nueva ocupa una celda libre; una placa repetida libera su celda.
#ifndef GENERADORPLACAS_H
#define GENERADORPLACAS_H

#include <string>
#include <map>
#include <vector>

class GeneradorPlacas {
public:
    explicit GeneradorPlacas(int totalCeldas);

    std::string siguienteMensaje();

private:
    std::string nuevaPlaca() const;
    std::string horaActual() const;

    std::map<std::string, int> ocupadas_;  // placa -> celda donde está
    std::vector<int> libres_;              // celdas disponibles
};

#endif

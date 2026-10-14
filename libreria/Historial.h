// Guarda cada evento recibido en un archivo CSV (separador ';').
#ifndef HISTORIAL_H
#define HISTORIAL_H

#include <string>

class Historial {
public:
    explicit Historial(const std::string& archivo);

    void guardar(const std::string& placa, const std::string& horaGenerador,
                 int celda, const std::string& resultado, int ocupadas);

private:
    std::string archivo_;
};

#endif

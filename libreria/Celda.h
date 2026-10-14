// Una celda (puesto) del parqueadero.
#ifndef CELDA_H
#define CELDA_H

#include <string>

class Celda {
public:
    Celda();
    explicit Celda(int numero);

    int numero() const;
    bool ocupada() const;
    std::string placa() const;
    std::string hora() const;

    void ocupar(const std::string& placa, const std::string& hora);
    void liberar();

private:
    int numero_;
    bool ocupada_;
    std::string placa_;
    std::string hora_;
};

#endif

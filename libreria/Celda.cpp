#include "Celda.h"

Celda::Celda() : numero_(0), ocupada_(false) {}

Celda::Celda(int numero) : numero_(numero), ocupada_(false) {}

int Celda::numero() const { return numero_; }
bool Celda::ocupada() const { return ocupada_; }
std::string Celda::placa() const { return placa_; }
std::string Celda::hora() const { return hora_; }

void Celda::ocupar(const std::string& placa, const std::string& hora) {
    ocupada_ = true;
    placa_ = placa;
    hora_ = hora;
}

void Celda::liberar() {
    ocupada_ = false;
    placa_ = "";
    hora_ = "";
}

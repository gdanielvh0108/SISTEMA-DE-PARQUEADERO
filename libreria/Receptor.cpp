#include "Receptor.h"
#include <sstream>
#include <cstdlib>

static std::string aTexto(int n) {
    std::ostringstream ss;
    ss << n;
    return ss.str();
}

Receptor::Receptor(int totalCeldas, const std::string& archivoCsv)
    : parqueadero_(totalCeldas), historial_(archivoCsv) {}

bool Receptor::iniciar(int puerto) { return servidor_.iniciar(puerto); }
void Receptor::detener() { servidor_.detener(); }

std::string Receptor::atender(int timeoutMs) {
    std::string mensaje = servidor_.recibir(timeoutMs);
    if (mensaje.empty()) return "";

    // Formato esperado: PLACA;HORA;CELDA
    std::string campos[3];
    std::stringstream ss(mensaje);
    std::string parte;
    int n = 0;
    while (n < 3 && std::getline(ss, parte, ';')) {
        campos[n++] = parte;
    }

    char* fin = NULL;
    long celda = std::strtol(campos[2].c_str(), &fin, 10);
    if (n < 3 || campos[0].empty() || fin == campos[2].c_str() || *fin != '\0') {
        servidor_.responder("ERROR;formato invalido");
        return "[!] Mensaje invalido: " + mensaje;
    }

    const std::string& placa = campos[0];
    const std::string& hora = campos[1];

    std::string resultado = parqueadero_.registrar(placa, hora, (int)celda);
    servidor_.responder(resultado + ";libres=" + aTexto(parqueadero_.libres()));
    historial_.guardar(placa, hora, (int)celda, resultado, parqueadero_.ocupadas());

    // Línea para mostrar en pantalla: "tipo;detalle" -> texto legible
    std::string tipo = resultado;
    std::string detalle;
    std::string::size_type pos = resultado.find(';');
    if (pos != std::string::npos) {
        tipo = resultado.substr(0, pos);
        detalle = resultado.substr(pos + 1);
    }

    std::string linea = "[" + hora + "] " + placa + " -> ";
    if (tipo == "ERROR") {
        linea += "ERROR: " + detalle;
    } else {
        linea += tipo + " celda " + detalle;
    }
    linea += " | Ocupadas: " + aTexto(parqueadero_.ocupadas()) + "/" + aTexto(parqueadero_.totalCeldas());
    return linea;
}

int Receptor::totalCeldas() const { return parqueadero_.totalCeldas(); }
int Receptor::ocupadas() const { return parqueadero_.ocupadas(); }
int Receptor::libres() const { return parqueadero_.libres(); }
bool Receptor::estaOcupada(int celda) const { return parqueadero_.estaOcupada(celda); }
std::string Receptor::placaEn(int celda) const { return parqueadero_.placaEn(celda); }
std::string Receptor::horaEn(int celda) const { return parqueadero_.horaEn(celda); }

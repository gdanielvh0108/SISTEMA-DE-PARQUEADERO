"""Visualizador del parqueadero (archivo principal).

Carga la librería dinámica parqueaderolib (C++ envuelto con SWIG), arranca
el servidor de sockets y abre la interfaz gráfica.
"""
import os
import sys
import tkinter as tk

# La librería compilada queda en ../libreria
RUTA_LIBRERIA = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "libreria")
sys.path.insert(0, RUTA_LIBRERIA)
if hasattr(os, "add_dll_directory"):  # Python 3.8+ en Windows
    os.add_dll_directory(os.path.abspath(RUTA_LIBRERIA))

try:
    import parqueaderolib
except ImportError as error:
    print("No se pudo cargar la librería dinámica:", error)
    print("Compílala primero (ver README): swig + python setup.py build_ext --inplace")
    sys.exit(1)

from interfaz import Interfaz

PUERTO = 8080
TOTAL_CELDAS = 20  # debe coincidir con el generador
ARCHIVO_CSV = "registro.csv"


def main():
    receptor = parqueaderolib.Receptor(TOTAL_CELDAS, ARCHIVO_CSV)

    if not receptor.iniciar(PUERTO):
        print(f"No se pudo iniciar el servidor en el puerto {PUERTO} "
              "(¿ya hay otro programa usándolo?)")
        return

    raiz = tk.Tk()
    Interfaz(raiz, receptor, PUERTO)
    raiz.mainloop()


if __name__ == "__main__":
    main()

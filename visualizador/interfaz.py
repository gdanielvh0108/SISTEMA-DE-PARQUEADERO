"""Interfaz gráfica del parqueadero (tkinter).

Muestra una cuadrícula con las celdas: verde = libre, rojo = ocupada.
Consulta periódicamente a la librería dinámica (Receptor, hecha en C++)
para recibir las placas y refrescar la pantalla.
"""
import tkinter as tk

COLOR_LIBRE = "#2e7d32"
COLOR_OCUPADA = "#c62828"
COLOR_FONDO = "#f4f4f4"


class Interfaz:
    def __init__(self, raiz, receptor, puerto, columnas=5):
        self.raiz = raiz
        self.receptor = receptor
        self.columnas = columnas
        self.celdas = {}  # número de celda -> Label

        raiz.title("Parqueadero")
        raiz.configure(bg=COLOR_FONDO)
        raiz.protocol("WM_DELETE_WINDOW", self.cerrar)

        tk.Label(raiz, text="Parqueadero", font=("Segoe UI", 18, "bold"),
                 bg=COLOR_FONDO).pack(pady=(10, 0))
        tk.Label(raiz, text=f"Escuchando en el puerto {puerto}",
                 font=("Segoe UI", 9), bg=COLOR_FONDO, fg="#555").pack()

        self.marco_celdas = tk.Frame(raiz, bg=COLOR_FONDO)
        self.marco_celdas.pack(padx=15, pady=10)
        self._crear_celdas()

        self.lbl_resumen = tk.Label(raiz, font=("Segoe UI", 11, "bold"), bg=COLOR_FONDO)
        self.lbl_resumen.pack()

        leyenda = tk.Frame(raiz, bg=COLOR_FONDO)
        leyenda.pack(pady=4)
        tk.Label(leyenda, text="  Libre  ", bg=COLOR_LIBRE, fg="white").pack(side="left", padx=4)
        tk.Label(leyenda, text="  Ocupada  ", bg=COLOR_OCUPADA, fg="white").pack(side="left", padx=4)

        self.lista = tk.Listbox(raiz, height=6, width=62, font=("Consolas", 9))
        self.lista.pack(padx=15, pady=(6, 12))

        self._actualizar()
        self.raiz.after(100, self._sondear)

    def _crear_celdas(self):
        for numero in range(1, self.receptor.totalCeldas() + 1):
            fila = (numero - 1) // self.columnas
            col = (numero - 1) % self.columnas
            lbl = tk.Label(self.marco_celdas, width=11, height=4, fg="white",
                           font=("Segoe UI", 9, "bold"), relief="raised", bd=2)
            lbl.grid(row=fila, column=col, padx=3, pady=3)
            self.celdas[numero] = lbl

    def _actualizar(self):
        """Redibuja todas las celdas con el estado actual de la librería."""
        for numero, lbl in self.celdas.items():
            if self.receptor.estaOcupada(numero):
                texto = f"Celda {numero}\n{self.receptor.placaEn(numero)}\n{self.receptor.horaEn(numero)}"
                lbl.configure(text=texto, bg=COLOR_OCUPADA)
            else:
                lbl.configure(text=f"Celda {numero}\nLibre", bg=COLOR_LIBRE)

        self.lbl_resumen.configure(
            text=f"Libres: {self.receptor.libres()}    Ocupadas: {self.receptor.ocupadas()}")

    def _sondear(self):
        """Pregunta a la librería si llegó una placa (espera máx. 50 ms)."""
        linea = self.receptor.atender(50)
        if linea:
            self.lista.insert(0, linea)
            self._actualizar()
        self.raiz.after(50, self._sondear)

    def cerrar(self):
        self.receptor.detener()
        self.raiz.destroy()

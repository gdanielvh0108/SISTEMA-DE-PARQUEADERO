# SISTEMA-DE-PARQUEADERO
Proyecto de comunicación por sockets entre dos procesos, con una librería dinámica hecha en C++ y envuelta con SWIG para usarla desde Python.
Generador de placas (C++): envía una placa cada 2 o 5 segundos por TCP.
Visualizador (Python): muestra una cuadrícula de celdas (verde = libre, rojo = ocupada). Recibe los datos a través de la librería dinámica `parqueaderolib` (C++).
Cada mensaje trae la placa, la hora y la celda que ocuparía el carro.
Si una placa se repite, se libera su celda.
AQUI VA UML

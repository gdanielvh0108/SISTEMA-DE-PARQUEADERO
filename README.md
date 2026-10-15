### SISTEMA-DE-PARQUEADERO
Sistema de parqueadero
Proyecto de comunicación por sockets entre dos procesos, con una librería dinámica hecha en C++ y envuelta con SWIG para usarla desde Python.
Generador de placas (C++): envía una placa cada 2 o 5 segundos por TCP.
Visualizador (Python): muestra una cuadrícula de celdas (verde = libre, rojo = ocupada). Recibe los datos a través de la librería dinámica `parqueaderolib` (C++).
Cada mensaje trae la placa, la hora y la celda que ocuparía el carro.
Si una placa se repite, se libera su celda.


# Estructura
```
generador/            Generador de placas (C++, Winsock)
  main.cpp              archivo principal
  GeneradorPlacas.*     crea los mensajes PLACA;HORA;CELDA
  ClienteSocket.*       envía por TCP
libreria/             Librería dinámica (C++ + SWIG)
  Receptor.*            clase que usa Python (socket + parqueadero + historial)
  ServidorSocket.*      servidor TCP
  Parqueadero.*         celdas y regla ocupar/liberar
  Celda.*               una celda
  Historial.*           guarda registro.csv
  parqueaderolib.i      interfaz SWIG
  setup.py              compila el módulo _parqueaderolib.pyd
visualizador/         Interfaz gráfica (Python, tkinter)
  main.py               archivo principal
  interfaz.py           clase Interfaz
docs/                 UML (uml.dot y img/uml.png) y textos de la wiki
```

Respuesta	`OCUPADA;celda;libres=N`	`OCUPADA;7;libres=19`
	`LIBERADA;celda;libres=N`	`LIBERADA;7;libres=20`
	`ERROR;motivo;libres=N`	`ERROR;celda ocupada por otra placa;libres=19`
Reglas: placa nueva → ocupa la celda; placa repetida → libera su celda. Se rechaza una celda fuera de rango (1 a 20) o ya ocupada por otra placa. Cada evento se guarda en `registro.csv`.

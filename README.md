# SISTEMA-DE-PARQUEADERO
Sistema de parqueadero
Proyecto de comunicación por sockets entre dos procesos, con una librería dinámica hecha en C++ y envuelta con SWIG para usarla desde Python.
Generador de placas (C++): envía una placa cada 2 o 5 segundos por TCP.
Visualizador (Python): muestra una cuadrícula de celdas (verde = libre, rojo = ocupada). Recibe los datos a través de la librería dinámica `parqueaderolib` (C++).
Cada mensaje trae la placa, la hora y la celda que ocuparía el carro.
Si una placa se repite, se libera su celda.

# Diagrama UML
<img width="1674" height="1285" alt="uml" src="https://github.com/user-attachments/assets/7750df58-3311-4f7c-b696-5b82a94b13b0" />



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


Respuesta	`OCUPADA;celda;libres=N`	`OCUPADA;7;libres=19`




	`LIBERADA;celda;libres=N`	`LIBERADA;7;libres=20`
	`ERROR;motivo;libres=N`	`ERROR;celda ocupada por otra placa;libres=19`

## Compilar

Librería (en `libreria/`):
```
swig -c++ -python parqueaderolib.i
python setup.py build_ext --inplace
```
Generador (en `generador/`):
```
g++ main.cpp GeneradorPlacas.cpp ClienteSocket.cpp -o generador.exe -lws2_32
```

## Ejecutar

1. Equipo del visualizador: abrir el puerto 8080 en el firewall y ejecutar `python main.py` en `visualizador/`.
2. Equipo del generador: ejecutar `generador.exe`, escribir la IP del otro equipo (`ipconfig`) y elegir 2 o 5 segundos.

## Protocolo

Mensaje: `PLACA;HORA;CELDA` (ejemplo: `ABC123;14:35:02;7`).
Placa nueva → ocupa la celda. Placa repetida → libera la celda.

## Autores

- Daniel Vergara
- Yireinis Renteria Martinez

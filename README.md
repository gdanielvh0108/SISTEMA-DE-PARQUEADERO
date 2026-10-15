# SISTEMA-DE-PARQUEADERO
Sistema de parqueadero
Proyecto de comunicación por sockets entre dos procesos, con una librería dinámica hecha en C++ y envuelta con SWIG para usarla desde Python.
Generador de placas (C++): envía una placa cada 2 o 5 segundos por TCP.
Visualizador (Python): muestra una cuadrícula de celdas (verde = libre, rojo = ocupada). Recibe los datos a través de la librería dinámica `parqueaderolib` (C++).
Cada mensaje trae la placa, la hora y la celda que ocuparía el carro.
Si una placa se repite, se libera su celda.
![Diagrama UML](docs/img/uml.png)
Estructura
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
Requisitos
Windows en ambos equipos y los dos en la misma red.
Equipo del visualizador: Python 3 (con tkinter, viene en el instalador oficial), SWIG, `pip install setuptools` y un compilador de C++ de la misma arquitectura que Python (Build Tools de Visual Studio o MinGW-w64).
Equipo del generador: un compilador de C++ (g++ o Dev-C++).
Compilar
Librería dinámica (en `libreria/`):
```
swig -c++ -python parqueaderolib.i
python setup.py build_ext --inplace
```
Con MinGW agrega `--compiler=mingw32` al segundo comando. Se genera `_parqueaderolib...pyd` (la DLL) y `parqueaderolib.py`.
Generador (en `generador/`):
```
g++ main.cpp GeneradorPlacas.cpp ClienteSocket.cpp -o generador.exe -lws2_32
```
En Dev-C++: crear un proyecto con esos tres `.cpp` y agregar `-lws2_32` en las opciones del enlazador.
Ejecutar
Equipo del visualizador: abrir el puerto 8080 en el firewall (PowerShell como administrador)
`New-NetFirewallRule -DisplayName "Puerto 8080" -Direction Inbound -Protocol TCP -LocalPort 8080 -Action Allow`
y ejecutar `python main.py` dentro de `visualizador/`. Se abre la ventana.
Obtener la IPv4 de ese equipo con `ipconfig`.
Equipo del generador: ejecutar `generador.exe`, escribir la IP anterior y elegir el intervalo (2 o 5 s).
Para probar en un solo equipo se usa `127.0.0.1` (Enter en el generador).
Protocolo
Sentido	Mensaje	Ejemplo
Generador → Visualizador	`PLACA;HORA;CELDA`	`ABC123;14:35:02;7`
Respuesta	`OCUPADA;celda;libres=N`	`OCUPADA;7;libres=19`
	`LIBERADA;celda;libres=N`	`LIBERADA;7;libres=20`
	`ERROR;motivo;libres=N`	`ERROR;celda ocupada por otra placa;libres=19`
Reglas: placa nueva → ocupa la celda; placa repetida → libera su celda. Se rechaza una celda fuera de rango (1 a 20) o ya ocupada por otra placa. Cada evento se guarda en `registro.csv`.

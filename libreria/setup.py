"""Compila la librería dinámica (_parqueaderolib.pyd) generada por SWIG.

    swig -c++ -python parqueaderolib.i
    python setup.py build_ext --inplace            (Visual Studio Build Tools)
    python setup.py build_ext --inplace --compiler=mingw32   (MinGW)
"""
import sys
from setuptools import setup, Extension

usa_mingw = any("mingw" in a for a in sys.argv)

modulo = Extension(
    "_parqueaderolib",
    sources=[
        "parqueaderolib_wrap.cxx",
        "Celda.cpp",
        "Parqueadero.cpp",
        "Historial.cpp",
        "ServidorSocket.cpp",
        "Receptor.cpp",
    ],
    libraries=["ws2_32"] if sys.platform == "win32" else [],
    # Con MinGW, enlaza el runtime de C++ dentro del .pyd para no depender de DLL sueltas
    extra_link_args=["-static-libgcc", "-static-libstdc++", "-static"] if usa_mingw else [],
    language="c++",
)

setup(name="parqueaderolib", version="1.0", ext_modules=[modulo])

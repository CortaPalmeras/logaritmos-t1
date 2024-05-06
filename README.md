
## Tarea 1 de Geometría Computacional

#### Requisitos para compilar y ejecutar la tarea:

- Un compilador de C++, puede ser `g++` o `clang++`.
- `make` para ejecutar los comandos.
- Alguna distribución de Linux, la tarea fue probada en Debian.

#### Instrucciones de ejecución:

El archivo makefile tiene targets para ejecutar los tests compilando con `g++` y con `clang++`:
- `g++`: Ejecutar `make` o `make test`.
- `clang++`: Ejecutar `make test-clang`.

#### Layout de la tarea:.

```
├── makefile
├── README.md
├── bin
├── include
│   ├── busqueda.hpp
│   ├── ciaccia-patella.hpp
│   └── tarea1.hpp
├── src
│   ├── busqueda.cpp
│   ├── ciaccia-patella.cpp
│   └── tarea1.cpp
└── test
    ├── busqueda-test.cpp
    ├── ciaccia-patella-test.cpp
    ├── ejecutar-tests.cpp
    └── semilla.cpp
```
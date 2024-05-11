
## Tarea 1 de Geometría Computacional

#### Requisitos para compilar y ejecutar la tarea:

- Un compilador de C++, puede ser `g++` o `clang++`.
- `make` para ejecutar los comandos.
- Alguna distribución de Linux, la tarea fue probada en Debian.
- `Python3` si se desea generar los gráficos mostrados en el informe.

#### Instrucciones de ejecución:

Para ejecutar la tarea solo es necesario ejecutar `make` en el directorio raiz
de la tarea, por defecto la compilación se debería realizar con g++, sobreescribir
este comportamiento se puede la opción `CXX` en el comando, por ejemplo: `make CXX=clang++`
va a hacer que se compile con `clang++`, es necesario que el compilador que se 
utilice acepte las mismas flags especificadas en el archivo makefile.

#### Layout de la tarea:

```
.
├── makefile
├── README.md
├── compile_flags.txt
├── bin
├── include
│   ├── busqueda.hpp
│   ├── ciaccia_patella.hpp
│   ├── sexton_swinbank.hpp
│   └── tarea.hpp
├── src
│   ├── busqueda.cpp
│   ├── ciaccia_patella.cpp
│   ├── minmax.cpp
│   ├── sexton_swinbank.cpp
│   └── tarea.cpp
└── test
    ├── busqueda_test.cpp
    ├── ciaccia_patella_test.cpp
    ├── minmax_test.cpp
    ├── sexton_swinbak_test.cpp
    ├── tarea_test.cpp
    └── tarea_test.hpp
```


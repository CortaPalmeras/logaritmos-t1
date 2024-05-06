
#ifndef BUSQUEDA
#define BUSQUEDA

#include <cmath>
#include <random>

#include "tarea1.hpp"

typedef struct tupla {
    Conjunto conjunto;
    int accesos;
} tupla;

void busqueda(Nodo &arbol, Query &consulta, tupla &Tupla);

double gen_coordenada() {
    return dist_0_1(rdg);
}

Conjunto *crear_conjunto_puntos(int cantidad);

ConjuntoQ *crear_conjunto_query();

#endif

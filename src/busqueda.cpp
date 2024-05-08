
#include <cmath>

#include "busqueda.hpp"
#include "tarea1.hpp"

int busqueda(Nodo &arbol, Query &consulta, Conjunto &puntos) {
    if (arbol.entradas[0].a == NULL) {
        for (int i = 0; i < arbol.size; i++) {
            if (distancia(arbol.entradas[i].p, consulta.p) <= consulta.r) {
                puntos.push_back(arbol.entradas[i].p);
            }
            Tupla.accesos++;
        }

    } else {
        for (int i = 0; i < arbol.size; i++) {
            if (distancia(arbol.entradas[i].p, consulta.p) <= arbol.entradas[i].r + consulta.r) {
                accesos = busqueda(*(arbol.entradas[i].a), consulta, puntos, accesos);
            }
            Tupla.accesos++;
        }
    }
}

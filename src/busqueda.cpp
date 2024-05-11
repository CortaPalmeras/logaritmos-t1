
#include <cmath>

#include "busqueda.hpp"
#include "tarea.hpp"

// Algoritmo de busqueda especificado en el enunciado.
//
// Recibe un arbol y una query y añade los puntos de la query
// al conjunto de puntos especificado en el tercer argumento.
//
// Retorna el numero de accesos a disco, calculado como la
// cantidad de nodos que se leen.
int busqueda(Nodo &arbol, Query &consulta, Conjunto &puntos) {
    int accesos = 1;

    if (arbol.entradas[0].a == NULL) {
        for (int i = 0; i < arbol.size; i++) {
            if (distancia(arbol.entradas[i].p, consulta.p) <= consulta.r) {
                puntos.push_back(arbol.entradas[i].p);
            }
        }

    } else {
        for (int i = 0; i < arbol.size; i++) {
            if (distancia(arbol.entradas[i].p, consulta.p) <= arbol.entradas[i].r + consulta.r) {
                accesos += busqueda(*(arbol.entradas[i].a), consulta, puntos);
            }
        }
    }

    return accesos;
}

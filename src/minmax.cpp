
#include <cmath>
#include <cstddef>
#include <limits>

#include "sexton_swinbank.hpp"
#include "minmax.hpp"
#include "tarea.hpp"

using namespace std;

void minmax_split(Conjunto &puntos, Conjunto &c1, Conjunto &c2) {
    double r_min = numeric_limits<double>::max();

    // calcular distancias entre todos los puntos,
    // para que no se calculen más de una vez en la función.
    mat_distancias distancias(puntos.size(), vector<double>(puntos.size()));

    for (uint i = 0; i < puntos.size(); i++) {
        distancias[i][i] = 0;

        for (uint j = i + 1; j < puntos.size(); j++) {
            double dist = distancia(puntos[i], puntos[j]);
            distancias[i][j] = dist;
            distancias[j][i] = dist;
        }
    }

    Conjunto *out1 = NULL;
    Conjunto *out2 = NULL;

    // Se crean dos arreglos que contienen los nodos para dos listas
    // doblemente enlazadas, en estas listas se insertaran los indices de 
    // los puntos ordenados por cercanía al par de puntos elegidos.
    dll *nodos_cola_1 = new dll[puntos.size()];
    dll *nodos_cola_2 = new dll[puntos.size()];

    // Se inicializan los indices de las colas
    for (uint i = 0; i < puntos.size(); i++) {
        nodos_cola_1[i].indice = i;
        nodos_cola_2[i].indice = i;
    }

    // para cada par de puntos:
    for (uint i = 0; i < puntos.size(); i++) {
        for (uint j = i + 1; j < puntos.size(); j++) {

            reiniciar_cola(nodos_cola_1, puntos.size());
            reiniciar_cola(nodos_cola_2, puntos.size());

            dll *primero1 = nodos_cola_1;
            dll *primero2 = nodos_cola_2;

            // Se van añadiendo los indices de los puntos de manera que queden
            // ordenados por distancia, en las dos listas enlazadas.
            for (uint n = 0; n < puntos.size(); n++) {
                if (n == i || n == j) continue;
                añadir_nodo(&primero1, nodos_cola_1 + n, distancias, i, n);
                añadir_nodo(&primero2, nodos_cola_2 + n, distancias, j, n);
            }

            Conjunto *out1_candidato = new Conjunto(puntos.size() - puntos.size() / 2);
            Conjunto *out2_candidato = new Conjunto(puntos.size() / 2);

            // Se añaden los puntos intercaladamente en los conjuntos,
            // se van extrayendo los puntos de las listas enlazadas como 
            // si estas fueran colas, cuando se extrae un indice de una de las
            // dos listas este se debe eliminar de la otra.
            for (int k = 0, fin = puntos.size() - 4; k < fin; k++) {
                if (k % 2 == 0) {
                    (*out1_candidato)[k / 2] = puntos[primero1->indice];
                    eliminar_nodo(nodos_cola_2 + primero1->indice, &primero2);
                    primero1 = primero1->siguiente;
                } else {
                    (*out2_candidato)[k / 2] = puntos[primero2->indice];
                    eliminar_nodo(nodos_cola_1 + primero2->indice, &primero1);
                    primero2 = primero2->siguiente;
                }
            }

            // El ultimo indice que se obtiene de cada cola corresponderá
            // al punto más lejano.
            int ultimo_indice_1 = primero1->indice;
            eliminar_nodo(nodos_cola_2 + primero1->indice, &primero2);
            int ultimo_indice_2 = primero2->indice;

            (*out1_candidato)[out1_candidato->size() - 1] = puntos[ultimo_indice_1];
            (*out2_candidato)[out2_candidato->size() - 2] = puntos[ultimo_indice_2];

            double r_candidato;
            if (distancias[i][ultimo_indice_1] < distancias[j][ultimo_indice_2]) {
                r_candidato = distancias[j][ultimo_indice_2];
            } else {
                r_candidato = distancias[i][ultimo_indice_1];
            }

            if (r_candidato < r_min) {
                if (out1 != NULL) {
                    delete out1;
                    delete out2;
                }
                r_min = r_candidato;
                out1 = out1_candidato;
                out2 = out2_candidato;
            } else {
                delete out1_candidato;
                delete out2_candidato;
            }
        }
    }

    delete[] nodos_cola_1;
    delete[] nodos_cola_2;

    c1 = std::move(*out1);
    c2 = std::move(*out2);

    delete out1;
    delete out2;
}

void reiniciar_cola(dll *cola, int tamaño) {
    for (int i = 0; i < tamaño; i++) {
        cola[i].previo = NULL;
        cola[i].siguiente = NULL;
    }
}

void añadir_primero(dll **primero, dll *nodo) {
    (*primero)->previo = nodo;
    nodo->siguiente = *primero;
    *primero = nodo;
}

void añadir_antes(dll *nodo_actual, dll *nodo) {
    nodo->previo = nodo_actual->previo;
    nodo->siguiente = nodo_actual;

    nodo_actual->previo->siguiente = nodo;
    nodo_actual->previo = nodo;
}

void añadir_nodo(dll **primero, dll *nodo, mat_distancias &d, int i, int n) {

    // Caso en el que la distancia con el punto n es menor 
    // a la distancia con el primer elemento de la lista.
    if (d[i][n] < d[i][(*primero)->indice]) {
        añadir_primero(primero, nodo);
        return;
    }

    // se avanza por las colas hasta encontrar donde debe ir el punto n.
    dll *nodo_actual = (*primero)->siguiente;

    while (true) {
        // caso: se encuentra un punto con mayor distancia
        if (d[i][n] < d[i][nodo_actual->indice]) {
            añadir_antes(nodo_actual, nodo);
            break;

        // caso: se llega al final de la lista
        } else if (nodo_actual->siguiente == NULL) {
            nodo_actual->siguiente = nodo;
            break;

        // caso general: se sigue avanzando por la lista
        } else {
            nodo_actual = nodo_actual->siguiente;
        }
    }
}

void eliminar_nodo(dll *nodo, dll **primero) {
    if (nodo == *primero) {
        *primero = nodo->siguiente;
    } else if (nodo->siguiente == NULL) {
        nodo->previo->siguiente = NULL;
    } else {
        nodo->previo->siguiente = nodo->siguiente;
        nodo->siguiente->previo = nodo->previo;
    }
}



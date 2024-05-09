
#include <cmath>
#include <cstddef>
#include <limits>
#include <vector>

#include "sexton-swinbank.hpp"
#include "tarea1.hpp"

using namespace std;

typedef vector<vector<double>> mat_distancias;

typedef struct doubly_linked_list {
    int indice = 0;
    doubly_linked_list *siguiente = NULL;
    doubly_linked_list *previo = NULL;
} dll;

inline void añadir_primero(dll **primero, dll *nodo) {
    (*primero)->previo = nodo;
    nodo->siguiente = *primero;
    *primero = nodo;
}

inline void añadir_antes(dll *nodo_actual, dll *nodo) {
    nodo->previo = nodo_actual->previo;
    nodo->siguiente = nodo_actual;

    nodo_actual->previo->siguiente = nodo;
    nodo_actual->previo = nodo;
}

inline void añadir_nodo(dll **primero, dll *nodo, mat_distancias &d, int i, int n) {

    // Caso en el quel la distancia con un punto
    if (d[i][n] < d[i][(*primero)->indice]) {
        añadir_primero(primero, nodo);
        return;
    }

    // se avanza por las colas hasta encontrar donde debe ir el punto n.
    dll *nodo_actual = (*primero)->siguiente;

    while (true) {
        if (d[i][n] < d[i][nodo_actual->indice]) {
            añadir_antes(nodo_actual, nodo);
            break;

        } else if (nodo_actual->siguiente == NULL) {
            nodo_actual->siguiente = nodo;
            break;

        } else {
            nodo_actual = nodo_actual->siguiente;
        }
    }
}

inline void eliminar_nodo(dll *nodo, dll **primero) {
    if (nodo == *primero) {
        *primero = nodo->siguiente;
    } else if (nodo->siguiente == NULL) {
        nodo->previo->siguiente = NULL;
    } else {
        nodo->previo->siguiente = nodo->siguiente;
        nodo->siguiente->previo = nodo->previo;
    }
}

void minmax_split(Conjunto &puntos, Conjunto **out1, Conjunto **out2) {
    double r_min = numeric_limits<double>::max();

    // calcular distancias entre todos los puntos,
    // para que no se calculen más de una vez en la función.
    mat_distancias distancias(puntos.size(), vector<double>(puntos.size()));

    for (int i = 0; i < puntos.size(); i++) {
        distancias[i][i] = 0;

        for (int j = i + 1; j < puntos.size(); j++) {
            double dist = distancia(puntos[i], puntos[j]);
            distancias[i][j] = dist;
            distancias[j][i] = dist;
        }
    }

    // para cada par de puntos:
    for (int i = 0; i < puntos.size(); i++) {
        for (int j = i + 1; j < puntos.size(); j++) {
            // se crean dos listas doblemente enlazadas de puntos ordenados
            // en relación a su distancia con los dos puntos elegidos.
            dll cola_distancias1[puntos.size()];
            dll cola_distancias2[puntos.size()];

            dll *primero1 = cola_distancias1;
            dll *primero2 = cola_distancias2;

            // Se van añadiendo los indices de los puntos uno por uno a las listas
            for (int n = 0; n < puntos.size(); n++) {
                if (n == i || n == j)
                    continue;

                cola_distancias1[n].indice = n;
                cola_distancias2[n].indice = n;

                añadir_nodo(&primero1, cola_distancias1 + n, distancias, i, n);
                añadir_nodo(&primero2, cola_distancias2 + n, distancias, j, n);
            }

            Conjunto *out1_candidato = new Conjunto(puntos.size() - puntos.size() / 2);
            Conjunto *out2_candidato = new Conjunto(puntos.size() / 2);

            // se añaden los puntos intercaladamente en los conjuntos.
            for (int k = 0, fin = puntos.size() - 2; k < fin; k++) {
                if (k % 2 == 0) {
                    (*out1_candidato)[k / 2] = puntos[primero1->indice];
                    eliminar_nodo(cola_distancias2 + primero1->indice, &primero2);
                    primero1 = primero1->siguiente;
                } else {
                    (*out2_candidato)[k / 2] = puntos[primero2->indice];
                    eliminar_nodo(cola_distancias1 + primero2->indice, &primero1);
                    primero2 = primero2->siguiente;
                }
            }

            int ultimo1 = primero1->indice;
            eliminar_nodo(cola_distancias2 + primero1->indice, &primero2);
            int ultimo2 = primero2->indice;

            (*out1_candidato)[out1_candidato->size() - 1] = puntos[ultimo1];
            (*out2_candidato)[out2_candidato->size() - 2] = puntos[ultimo2];

            double r_candidato;
            if (distancias[i][ultimo1] < distancias[j][ultimo2]) {
                r_candidato = distancias[j][ultimo2];
            } else {
                r_candidato = distancias[i][ultimo1];
            }

            if (r_candidato < r_min) {
                if (out1 != NULL) {
                    delete out1;
                    delete out2;
                }
                r_min = r_candidato;
                *out1 = out1_candidato;
                *out2 = out2_candidato;
            } else {
                delete out1_candidato;
                delete out2_candidato;
            }
        }
    }
}


#include <cmath>
#include <cstddef>
#include <limits>
#include <map>
#include <random>
#include <vector>

#include "sexton-swinbank.hpp"
#include "tarea1.hpp"

using namespace std;
typedef struct dll {
    int indice = 0;
    dll *siguiente = NULL;
    dll *previo = NULL;
} doubly_linked_list;

void MinMaxSplit(Conjunto &puntos, Conjunto **out1, Conjunto **out2) {
    double r_min = numeric_limits<double>::max();

    // calcular distancias entre todos los puntos,
    // para que no se calculen más de una vez en la función.
    vector< vector<double> > d(puntos.size(), vector<double>(puntos.size()));

    for (int i = 0; i < puntos.size(); i++) {
        d[i][i] = 0;

        for (int j = i + 1; j < puntos.size(); j++) {
            double dist = distancia(puntos[i], puntos[j]);
            d[i][j] = dist;
            d[j][i] = dist;
        }
    }


    // para cada par de puntos:
    for (int i = 0; i < puntos.size(); i++) {
        for (int j = i + 1; j < puntos.size(); j++) {

            // se crean dos listas doblemente enlazadas de puntos ordenados 
            // en relación a su distancia con los dos puntos elegidos.
            doubly_linked_list cola_distancias1[puntos.size()];
            doubly_linked_list cola_distancias2[puntos.size()];

            doubly_linked_list *primero1 = cola_distancias1;
            doubly_linked_list *primero2 = cola_distancias2;

            // Se van añadiendo los indices de los puntos uno por uno a las colas
            for (int n = 0; n < puntos.size(); n++) {
                if (n == i || n == j) continue;

                cola_distancias1[n].indice = n;
                cola_distancias2[n].indice = n;

                bool falta1 = true;
                bool falta2 = true;

                // Caso en el quel la distancia con un punto 
                // sea menor que la del primer punto de la cola:
                if (d[i][n] < d[i][primero1->indice]) {
                    primero1->previo = cola_distancias1 + n;
                    cola_distancias1[n].siguiente = primero1;
                    primero1 = cola_distancias1 + n;
                    falta1 = false;
                }

                if (d[j][n] < d[j][primero2->indice]) {
                    primero2->previo = cola_distancias1 + n;
                    cola_distancias2[n].siguiente = primero1;
                    primero2 = cola_distancias1 + n;
                    falta2 = false;
                }
                
                doubly_linked_list *nodo_actual = primero1->siguiente;

                // se avanza por las colas hasta encontrar donde debe ir el punto n.
                while (falta1) {
                    if (d[i][n] < d[i][nodo_actual->indice]) {
                        nodo_actual->previo->siguiente = cola_distancias1 + n;
                        nodo_actual->previo = cola_distancias1 + n;
                        cola_distancias1[n].siguiente = nodo_actual;
                        falta1 = false;

                    } else if (nodo_actual->siguiente == NULL) {
                        nodo_actual->siguiente = cola_distancias1 + n;
                        nodo_actual = cola_distancias1 + n;
                        falta1 = false;

                    } else {
                        nodo_actual = nodo_actual->siguiente;
                    }
                }

                nodo_actual = primero2->siguiente;

                while (true) {
                    if (d[j][n] < d[j][nodo_actual->indice]) {
                        nodo_actual->previo->siguiente = cola_distancias2 + n;
                        nodo_actual->previo = cola_distancias2 + n;
                        cola_distancias2[n].siguiente = nodo_actual;
                        falta2 = false;

                    } else if (nodo_actual->siguiente == NULL) {
                        nodo_actual->siguiente = cola_distancias2 + n;
                        nodo_actual = cola_distancias2 + n;
                        falta2 = false;

                    } else {
                        nodo_actual = nodo_actual->siguiente;
                    }
                }
            }
        }
    }
}

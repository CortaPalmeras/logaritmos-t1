
#ifndef MINMAX
#define MINMAX

#include <vector>

#include "tarea.hpp"

typedef std::vector<std::vector<double>> mat_distancias;

typedef struct doubly_linked_list {
    int indice = 0;
    doubly_linked_list *siguiente = NULL;
    doubly_linked_list *previo = NULL;
} dll;

void minmax_split(Conjunto &puntos, Conjunto &c1, Conjunto &c2);

void reiniciar_cola(dll *cola, int tamaño);

void añadir_nodo(dll *primero, dll *nodo, mat_distancias const &d, int i, int n);

void eliminar_nodo(dll *nodo, dll **primero);

#endif // !MINMAX

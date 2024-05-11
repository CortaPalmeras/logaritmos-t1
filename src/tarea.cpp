
#include <cmath>
#include <iostream>
#include <random>

#include "tarea.hpp"

Nodo *crear_nodo() {
    Nodo *n = new Nodo;
    n->size = 0;
    return n;
}

Nodo *crear_nodo(Conjunto &puntos) {
    Nodo *n = new Nodo;
    n->size = 0;
    for (auto p : puntos) {
        añadir_entrada(n, p);
    }
    return n;
}

void añadir_entrada(Nodo *n, Punto p) {
    if (n->size >= B) {
        std::cout << "nodo sobrepasa el tamaño máximo en 'añadir_entrada'\n";
        exit(1);
    }
    int s = n->size;
    n->entradas[s].p = p;
    n->entradas[s].a = NULL;
    n->entradas[s].r = 0;
    n->size++;
}

void eliminar_arbol(Nodo *arbol) {
    if (arbol->entradas[0].a != NULL) {
        for (int i = 0; i < arbol->size; i++) {
            eliminar_arbol(arbol->entradas[i].a);
        }
    }
    delete arbol;
}

int altura_arbol(Nodo const &arbol) {
    if (arbol.entradas[0].a == NULL) {
        return 1;
    } else {
        return altura_arbol(*arbol.entradas[0].a) + 1;
    }
}

std::mt19937 rdg(GLOBAL_RANDOM_SEED);
std::uniform_real_distribution<double> dist_double_0_1(0.0, 1.0);
std::uniform_real_distribution<double> dist_double_0_inf(0.0, std::numeric_limits<double>::max());
std::uniform_int_distribution<int> dist_int_0_inf(0.0, std::numeric_limits<int>::max());

double random_double_0_a_1() {
    return dist_double_0_1(rdg);
}

double random_double_positivo() {
    return dist_double_0_inf(rdg);
}

int random_int_positivo() {
    return dist_int_0_inf(rdg);
}

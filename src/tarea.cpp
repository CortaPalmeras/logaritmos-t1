
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "tarea.hpp"

Nodo *crear_nodo() {
    Nodo *n = new Nodo;
    n->size = 0;
    return n;
}

Nodo *crear_nodo(Conjunto const &puntos) {
    Nodo *n = crear_nodo();

    for (auto p : puntos) {
        añadir_entrada(n, p);
    }

    return n;
}

Nodo *crear_nodo(std::vector<Entry> const &entradas) {
    Nodo *C = crear_nodo();
    
    for (auto entrada : entradas) {
        añadir_entrada(C, entrada);
    }
    
    return C;
}

void añadir_entrada(Nodo *n, Entry e) {
    if (n->size >= B) {
        std::cout << "nodo sobrepasa el tamaño máximo en 'añadir_entrada'\n";
        exit(1);
    }

    n->entradas[n->size] = e;
    n->size++;
}

void añadir_entrada(Nodo *n, Punto p) {
    if (n->size >= B) {
        std::cout << "nodo sobrepasa el tamaño máximo en 'añadir_entrada'\n";
        exit(1);
    }

    int s = n->size;
    n->entradas[s].p = p;
    n->entradas[s].r = 0;
    n->entradas[s].a = NULL;
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

std::mt19937 rng(GLOBAL_DEFAULT_RANDOM_SEED);
std::uniform_real_distribution<double> dist_double_0_1(0.0, 1.0);
std::uniform_real_distribution<double> dist_double_0_inf(0.0, std::numeric_limits<double>::max());
std::uniform_int_distribution<int> dist_int_0_inf(0.0, std::numeric_limits<int>::max());

double random_double_0_a_1() {
    return dist_double_0_1(rng);
}

double random_double_positivo() {
    return dist_double_0_inf(rng);
}

int random_int_positivo() {
    return dist_int_0_inf(rng);
}

void reiniciar_rng() {
    rng.seed(GLOBAL_DEFAULT_RANDOM_SEED);
}

void reiniciar_rng(int seed) {
    rng.seed(seed);
}


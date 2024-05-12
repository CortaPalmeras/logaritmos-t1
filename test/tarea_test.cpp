
#include <cassert>
#include <unordered_map>
#include <vector>

#include "tarea.hpp"
#include "tarea_test.hpp"

using namespace std;

// Funciones auxiliares para los tests.
int test_estrucutra_correcta_recur(Nodo *arbol) {
    if (arbol == NULL) return 0;

    assert(b <= arbol->size && arbol->size <= B);

    int h = test_estrucutra_correcta_recur(arbol->entradas[0].a);

    for (int i = 1; i < arbol->size; i++) {
        assert(h == test_estrucutra_correcta_recur(arbol->entradas[i].a));
    }

    return h + 1;
}

void verificar_puntos(Nodo &arbol, unordered_map<Punto, int, PuntoHash> &arbol_hash) {
    if (arbol.entradas[0].a == NULL) {
        for (int i = 0; i < arbol.size; i++) {
            arbol_hash[arbol.entradas[i].p] = 1;
        }

    } else {
        for (int i = 0; i < arbol.size; i++) {
            verificar_puntos(*arbol.entradas[i].a, arbol_hash);
        }
    }
}

uint contar_puntos(Nodo &arbol) {
    if (arbol.entradas[0].a == NULL) {
        return arbol.size;

    } else {
        uint suma = 0;
        for (int i = 0; i < arbol.size; i++) {
            suma += contar_puntos(*arbol.entradas[i].a);
        }
        return suma;
    }
}

// Este test checkea que un nodo tenga la estructura correcta de M-Tree,
// es decir, numero correcto de entradas por nodo y que esté valanceado.
void test_estrucutra_correcta(Nodo &arbol) {
    assert(arbol.size <= B);

    int h = test_estrucutra_correcta_recur(arbol.entradas[0].a);

    for (int i = 1; i < arbol.size; i++) {
        assert(h == test_estrucutra_correcta_recur(arbol.entradas[i].a));
    }
}

// Este test checkea que todos los puntos se hayan insertado correctamente
// en un M-Tree
void test_puntos_correctos(Nodo &arbol, Conjunto &puntos) {
    assert(contar_puntos(arbol) == puntos.size());

    unordered_map<Punto, int, PuntoHash> arbol_hash;
    verificar_puntos(arbol, arbol_hash);

    for (Punto &p : puntos) {
        assert(arbol_hash[p] == 1);
    }
}

Conjunto *generar_conjunto_puntos(int cantidad) {
    Conjunto *resultado = new Conjunto(cantidad);

    for (int i = 0; i < cantidad; i++) {
        (*resultado)[i].x = random_double_0_a_1();
        (*resultado)[i].y = random_double_0_a_1();
    }

    return resultado;
}

void eliminar_conjunto_puntos(Conjunto *puntos) {
    delete puntos;
}

vector<Query> *generar_conjunto_queries(int cantidad) {
    vector<Query> *queries = new vector<Query>(cantidad);

    for (int i = 0; i < cantidad; i++) {
        (*queries)[i].p.x = random_double_0_a_1();
        (*queries)[i].p.y = random_double_0_a_1();
        (*queries)[i].r = 0.2;
    }

    return queries;
}

void eliminar_conjunto_queries(vector<Query> *queries) {
    delete queries;
}

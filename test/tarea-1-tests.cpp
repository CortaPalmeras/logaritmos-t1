
#include <vector>

#include "tarea1.hpp"
#include "tarea-1-tests.hpp"

using namespace std;

int main(int argc, char **argv) {

    test_ciaccia_patella();
    test_sexton_swinbank();
    test_busqueda();

    return 0;
};

// Este test checkea que un nodo tenga la estructura correcta de M-Tree,
//es decir, numero correcto de entradas por nodo y que esté valanceado.
void test_estrucutra_correcta(Nodo &arbol) {}

// Este test checkea que los radios de las entradas esten correctos.
void test_puntos_correctos(Nodo &arbol, Conjunto &puntos) {}


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



#include <cassert>
#include <iostream>
#include <unordered_map>

#include "minmax.hpp"
#include "tarea.hpp"
#include "tarea_test.hpp"

using namespace std;

int main(void) {
    cout << "\n --- TEST: MIN-MAX SPLIT POLICY ---\n"
         << endl;

    for (int i = 10; i <= 2 * B; i += 10) {
        cout << "Testeando con " << i << " puntos:" << endl;
        Conjunto *puntos = generar_conjunto_puntos(i);
        Conjunto c1;
        Conjunto c2;

        minmax_split(*puntos, c1, c2);

        cout << "Test de cantidad...  ";
        assert(c1.size() + c2.size() == puntos->size());
        cout << "Pasado!" << endl;

        cout << "Test de puntos...    ";
        unordered_map<Punto, int, PuntoHash> c1_map, c2_map;

        for (Punto &p : c1) c1_map[p] = 1;

        for (Punto &p : c2) c2_map[p] = 2;

        for (Punto &p : *puntos) {
            assert(c1_map[p] == 1 || c2_map[p] == 2);
        }
        cout << "Pasado!\n"
             << endl;

        eliminar_conjunto_puntos(puntos);
    }

    return 0;
}

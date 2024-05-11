
#include <cmath>
#include <iostream>

#include "ciaccia_patella.hpp"
#include "tarea.hpp"
#include "tarea_test.hpp"

using namespace std;

int main(void) {
    cout << " --- TEST: CIACCIA-PATELLA ---\n" << endl;

    for (int i = 10; i <= 25; i++) {
        cout << "Testeando con 2^" << i << " puntos." << endl;
        Conjunto *puntos = generar_conjunto_puntos(pow(2, i));
        Nodo *arbol = ciaccia_patella(*puntos);

        cout << "Test estructural... ";
        test_estrucutra_correcta(*arbol);
        cout << "Pasado!" << endl;

        cout << "Test de puntos...   ";
        test_puntos_correctos(*arbol, *puntos);
        cout << "Pasado!\n" << endl;

        eliminar_conjunto_puntos(puntos);
        eliminar_arbol(arbol);
    }

    return 0;
}

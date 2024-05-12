
#include <cmath>
#include <iostream>

#include "sexton_swinbank.hpp"
#include "tarea.hpp"
#include "tarea_test.hpp"

using namespace std;

int main(void) {
    cout << " --- TEST: SEXTON-SWINBANK ---\n"
         << endl;

    for (int i = 10; i <= 25; i++) {
        cout << "Testeando con 2^" << i << " puntos." << endl;
        Conjunto *puntos = generar_conjunto_puntos(pow(2, i));
        Nodo *arbol = sexton_swinbank(*puntos);

        cout << "Test estructural... ";
        test_estrucutra_correcta(*arbol);
        cout << "Pasado!" << endl;

        cout << "Test de puntos...   ";
        test_puntos_correctos(*arbol, *puntos);
        cout << "Pasado!\n"
             << endl;

        eliminar_conjunto_puntos(puntos);
        eliminar_arbol(arbol);
    }
}


#include <cmath>
#include <iostream>

#include "ciaccia_patella.hpp"
#include "tarea.hpp"
#include "tarea_test.hpp"

using namespace std;

int main(void) {
    cout << " --- TEST: CIACCIA-PATELLA ---\n\n";

    cout << "Testeando con n = 2^" << 10 << endl;
    Conjunto *puntos = generar_conjunto_puntos(pow(2, 10));
    Nodo *arbol = ciaccia_patella(*puntos);

    cout << "Test estructural:" << endl;
    test_estrucutra_correcta(*arbol);

    cout << "Test de puntos:" << endl;
    test_puntos_correctos(*arbol, *puntos);

    eliminar_conjunto_puntos(puntos);
    eliminar_arbol(arbol);

    //    for (int i = 10; i < 25; i++) {
    //        std::cout << "Testeando con n = 2^" << i << "\n";
    //        Conjunto *puntos = generar_conjunto_puntos(pow(2, i));
    //        Nodo *arbol = ciaccia_patella(*puntos);
    //
    //        std::cout << "Test estructural:";
    //        test_estrucutra_correcta(*arbol);
    //
    //        std::cout << "Test de puntos:";
    //        test_puntos_correctos(*arbol, *puntos);
    //
    //        eliminar_conjunto_puntos(puntos);
    //        eliminar_arbol(arbol);
    //    }

    return 0;
}


#include <cmath>
#include <iostream>

#include "tarea1.hpp"
#include "tarea-1-tests.hpp"
#include "sexton-swinbank.hpp"

void test_sexton_swinbank() {
    std::cout << "--- TEST: SEXTON-SWINBANK ---\n\n";

    for (int i = 10; i < 25; i++) {
        std::cout << "Testeando con n = 2^" << i << "\n";
        Conjunto *puntos = generar_conjunto_puntos(pow(2, i));
        Nodo *arbol = sexton_swinbank(*puntos);
        
        std::cout << "Test estructural:";
        test_estrucutra_correcta(*arbol);

        std::cout << "Test de puntos:";
        test_puntos_correctos(*arbol, *puntos);

        eliminar_conjunto_puntos(puntos);
        eliminar_arbol(arbol);
    }
}

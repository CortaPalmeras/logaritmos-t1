
#ifndef TESTS
#define TESTS

#include <vector>

#include "tarea.hpp"

// Tests utilizados para probar los constructores de un arbol.
void test_estrucutra_correcta(Nodo &arbol);
void test_puntos_correctos(Nodo &arbol, Conjunto &puntos);

// Funciónes auxiliares utilizadas para generar sets de prueba.
Conjunto *generar_conjunto_puntos(int cantidad);
void eliminar_conjunto_puntos(Conjunto *puntos);
std::vector<Query> *generar_conjunto_queries(int cantidad);
void eliminar_conjunto_queries(std::vector<Query> *queries);

#endif


#ifndef TESTS
#define TESTS

#include <vector>

#include "tarea1.hpp"

// Funciónes auxiliares utilizadas para generar sets de prueba.
Conjunto *generar_conjunto_puntos(int cantidad);
void eliminar_conjunto_puntos(Conjunto *puntos);
vector<Query> *generar_conjunto_queries(int cantidad);
void eliminar_conjunto_queries(vector<Query> *queries);

// Tests utilizados para probal los constructores de un arbol.
void test_puntos_correctos(Nodo &arbol) {}
void test_estrucutra_correcta(Nodo &arbol) {}

// Tests utilizados para probar los algoritmos implementados.
void test_ciaccia_patella();
void test_sexton_swinbank();
void test_busqueda();

#endif
// Archivo con funciónes untilizadas en el algoritmo de ciaccia-patella.

#ifndef CIACCIA_PATELLA
#define CIACCIA_PATELLA

#include <map>

#include "tarea1.hpp"

Nodo *ciaccia_patella(Conjunto const &p_in);

Conjunto *random_sample(Conjunto const &puntos, int k);
Particion *crear_particion(Conjunto const &puntos, Conjunto const &samples);
void asignar_puntos_a_samples(Conjunto const &puntos, Conjunto const &samples, Particion &particion);

void extraer_subarboles(Nodo *arbol, std::map<Punto, Nodo *> &raices, Conjunto &puntos, int dif);
void unir_subarboles(Nodo *arbol, std::map<Punto, Nodo *> &subarboles);

void asignar_distancias_arbol(Nodo &arbol);
void asignar_distancias_entrada(Entry &entrada);

#endif

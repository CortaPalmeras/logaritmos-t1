
#include <vector>
#include <map>

#include "tarea.hpp"

typedef struct {
    Conjunto a;
    Conjunto b;
    int indiceA;
    int indiceB;
} dosConjuntos;

Nodo *sexton_swinbank(Conjunto &puntos);

void elegir_medoide(Conjunto &c_in, std::map<Conjunto, Punto> &medoide);

void elegirParesCercanos(Particion c, std::map<Conjunto, Punto> medoide,
                         std::map<Conjunto, int> tamaño_conjuntos, dosConjuntos &resultado);

std::vector<Conjunto> crear_clusters(Conjunto &c_in);

Entry OutPutHoja(Conjunto &c_in);

Entry OutPutInterno(std::vector<Entry> c_mra);


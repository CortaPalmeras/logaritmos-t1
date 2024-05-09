
#include <vector>

#include "tarea1.hpp"

Nodo *sexton_swinbank(Conjunto &puntos);

std::vector<Conjunto> crear_clusters(Conjunto &puntos);

Entry output_hoja(Conjunto &puntos);

Entry output_interno(std::vector<Entry> &tuplas);

void minmax_split(Conjunto &puntos, Conjunto **out1, Conjunto **out2);

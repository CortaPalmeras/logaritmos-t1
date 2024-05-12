
#include <vector>

#include "tarea.hpp"

Nodo *sexton_swinbank(Conjunto &puntos);

// Retorna el indice del medoide primario de un conjunto de puntos
// y el radio cobertor que este genera sobre el conjunto.
std::pair<int, double> elegir_medoide(Conjunto const &puntos);

Entry output_hoja(Conjunto &c_in);

Entry output_interno(std::vector<Entry> const &c_mra);

std::pair<int,int> elegir_pares_cercanos(Particion const &clusters, std::vector<int> const &medoides);

Particion *crear_clusters(Conjunto &c_in);


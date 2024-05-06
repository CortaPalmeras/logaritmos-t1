
#include "busqueda.hpp"

#include "tests.hpp"

using namespace std;


void crear_conjuntos_random() {

    for(int i =0;i<15;i++){
        conjuntos[i] = crear_conjunto_puntos(pow(2,10+i));
    }

    querys = crear_conjunto_query();
}

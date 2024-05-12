
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

#include "busqueda.hpp"
#include "ciaccia_patella.hpp"
#include "sexton_swinbank.hpp"
#include "tarea.hpp"
#include "tarea_test.hpp"
#include "fstream"
using namespace std;

vector<vector<int>> matriz_accesos(16, std::vector<int>(100));
vector<vector<int>> matriz_tiempos(16, std::vector<int>(100));

void test_tiempos_accesos() {
    cout << " --- TEST: TIEMPOS Y ACCESOS DE BUSQUEDA ---\n\n";

    for (int i = 10; i <= 25; i++) {
        std::cout << "Testeando con n = 2^" << i << "\n";

        Conjunto *puntos = generar_conjunto_puntos(pow(2, i));
        Nodo *arbol = ciaccia_patella(*puntos);
        vector<Query> *queries = generar_conjunto_queries(100);

        for (int j = 0; j < 100; j++) {
            auto inicio = chrono::high_resolution_clock::now();

            Conjunto output_busqueda;
            matriz_accesos[i][j] = busqueda(*arbol, (*queries)[j], output_busqueda);

            auto fin = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
            matriz_tiempos[i][j] = duracion.count();
        }

        eliminar_arbol(arbol);
        eliminar_conjunto_puntos(puntos);
        eliminar_conjunto_queries(queries);
    }
}

void crear_txts(){
    ofstream archivo1("archivo_accesos");
    ofstream archivo2("archivo_tiempos");
    int potencia=10;
    for (int i=0;i<16;i++){
        archivo1<<"arbol 2 a la "+to_string(potencia)+"\n";
        archivo2<<"arbol 2 a la "+to_string(potencia)+"\n";
        potencia++;
        for(int j=0;j<100;j++){
            archivo1<<to_string(matriz_accesos[i][j])+"\n";
            archivo2<<to_string(matriz_tiempos[i][j])+"\n";
        }
    }
    archivo1.close();
    archivo2.close();
}


int main(void) {
    test_tiempos_accesos();
    crear_txts();
}

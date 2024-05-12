
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

#include "busqueda.hpp"
#include "ciaccia_patella.hpp"
#include "fstream"
#include "sexton_swinbank.hpp"
#include "tarea.hpp"
#include "tarea_test.hpp"
using namespace std;

vector<vector<int>> cp_accesos(16, std::vector<int>(100));
vector<vector<int>> cp_tiempos(16, std::vector<int>(100));

vector<vector<int>> ss_accesos(16, std::vector<int>(100));
vector<vector<int>> ss_tiempos(16, std::vector<int>(100));

void test_tiempos_accesos() {
    cout << " --- TEST: TIEMPOS Y ACCESOS DE BUSQUEDA ---\n\n";

    for (int i = 0; i <= 15; i++) {
        int n = i + 10;
        std::cout << "Testeando con 2^" << n << " puntos\n";

        Conjunto *puntos = generar_conjunto_puntos(pow(2, n));
        vector<Query> *queries = generar_conjunto_queries(100);


        cout << "Con arbol generado con Ciaccia-Patella: ";
        Nodo *arbol = ciaccia_patella(*puntos);
        for (int j = 0; j < 100; j++) {
            auto inicio = chrono::high_resolution_clock::now();

            Conjunto output_busqueda;
            cp_accesos[i][j] = busqueda(*arbol, (*queries)[j], output_busqueda);

            auto fin = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
            cp_tiempos[i][j] = duracion.count();
        }
        eliminar_arbol(arbol);
        cout << "Listo." << endl;
        

        cout << "Con arbol generado con Sexton-Swinbank: ";
        arbol = sexton_swinbank(*puntos);
        for (int j = 0; j < 100; j++) {
            auto inicio = chrono::high_resolution_clock::now();

            Conjunto output_busqueda;
            ss_accesos[i][j] = busqueda(*arbol, (*queries)[j], output_busqueda);

            auto fin = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
            ss_tiempos[i][j] = duracion.count();
        }
        eliminar_arbol(arbol);
        cout << "Listo." << endl;


        eliminar_conjunto_puntos(puntos);
        eliminar_conjunto_queries(queries);
    }
}

void crear_txts() {
    ofstream archivo1("resultados/cp_accesos.txt");
    ofstream archivo2("resultados/cp_tiempos.txt");
    int potencia = 10;
    for (int i = 0; i < 16; i++) {
        archivo1 << "arbol 2 a la " + to_string(potencia) + "\n";
        archivo2 << "arbol 2 a la " + to_string(potencia) + "\n";
        potencia++;
        for (int j = 0; j < 100; j++) {
            archivo1 << to_string(cp_accesos[i][j]) + "\n";
            archivo2 << to_string(cp_tiempos[i][j]) + "\n";
        }
    }
    archivo1.close();
    archivo2.close();
}

int main(void) {
    test_tiempos_accesos();
    crear_txts();
}

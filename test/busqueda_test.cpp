
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "busqueda.hpp"
#include "ciaccia_patella.hpp"
#include "fstream"
#include "sexton_swinbank.hpp"
#include "tarea.hpp"
#include "tarea_test.hpp"

using namespace std;


#define TESTEAR_CIACCIA_PATELLA 0
#define TESTEAR_SEXTON_SWINBANK 1

void vaciar_archivo(string nombre) {
    ofstream archivo(nombre);
    archivo.close();
}

int main(void) {

    cout << " --- TEST: TIEMPOS Y ACCESOS DE BUSQUEDA ---" << endl;

     #if TESTEAR_CIACCIA_PATELLA

    cout << "\nGenerando arboles con Ciaccia-Patella:" << endl;

    vaciar_archivo("./resultados/accesos_cp.txt");
    vaciar_archivo("./resultados/tiempos_cp.txt");

    for (int i = 0; i <= 15; i++) {

        reiniciar_rng(GLOBAL_DEFAULT_RANDOM_SEED * i);

        int n = i + 10;
        Conjunto *puntos = generar_conjunto_puntos(pow(2, n));
        vector<Query> *queries = generar_conjunto_queries(100);

        ofstream archivo_accesos_cp("./resultados/accesos_cp.txt", ios::app);
        ofstream archivo_tiempos_cp("./resultados/tiempos_cp.txt", ios::app);

        archivo_accesos_cp << "arbol 2^" + to_string(n) + " puntos\n";
        archivo_tiempos_cp << "arbol 2^" + to_string(n) + " puntos\n";

        std::cout << "Testeando con 2^" << n << " puntos: ";

        Nodo *arbol = ciaccia_patella(*puntos);

        for (int j = 0; j < 100; j++) {
            Conjunto output_busqueda;
            auto inicio = chrono::high_resolution_clock::now();

            archivo_accesos_cp << to_string(busqueda(*arbol, (*queries)[j], output_busqueda)) + "\n";

            auto fin = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<std::chrono::microseconds>(fin - inicio);
            archivo_tiempos_cp << to_string(duracion.count()) + "\n";
        }

        cout << "Listo." << endl;

        eliminar_arbol(arbol);
        eliminar_conjunto_puntos(puntos);
        eliminar_conjunto_queries(queries);

        archivo_accesos_cp.close();
        archivo_tiempos_cp.close();
    }

    #endif // TESTEAR_CIACCIA_PATELLA

    #if TESTEAR_SEXTON_SWINBANK

    cout << "\nBusqueda en aarboles generados con Sexton-Swinbank:" << endl;

    vaciar_archivo("./resultados/accesos_ss.txt");
    vaciar_archivo("./resultados/tiempos_ss.txt");

    for (int i = 0; i <= 15; i++) {

        reiniciar_rng(GLOBAL_DEFAULT_RANDOM_SEED * i);

        int n = i + 10;
        Conjunto *puntos = generar_conjunto_puntos(pow(2, n));
        vector<Query> *queries = generar_conjunto_queries(100);

        ofstream archivo_accesos_ss("./resultados/accesos_ss.txt", ios::app);
        ofstream archivo_tiempos_ss("./resultados/tiempos_ss.txt", ios::app);

        archivo_accesos_ss << "arbol 2^" + to_string(n) + " puntos.\n";
        archivo_tiempos_ss << "arbol 2^" + to_string(n) + " puntos.\n";

        std::cout << "Testeando con 2^" << n << " puntos: ";

        Nodo *arbol = sexton_swinbank(*puntos);

        for (int j = 0; j < 100; j++) {
            Conjunto output_busqueda;
            auto inicio = chrono::high_resolution_clock::now();

            archivo_accesos_ss << to_string(busqueda(*arbol, (*queries)[j], output_busqueda)) + "\n";

            auto fin = chrono::high_resolution_clock::now();
            auto duracion = chrono::duration_cast<std::chrono::microseconds>(fin - inicio);

            archivo_tiempos_ss << to_string(duracion.count()) + "\n";
        }

        cout << "Listo." << endl;

        eliminar_arbol(arbol);
        eliminar_conjunto_puntos(puntos);
        eliminar_conjunto_queries(queries);

        archivo_accesos_ss.close();
        archivo_tiempos_ss.close();
    }

    #endif //TESTEAR_SEXTON_SWINBANK

    return 0;
}

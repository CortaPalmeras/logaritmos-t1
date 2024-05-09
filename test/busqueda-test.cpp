
#include <chrono>
#include <cmath>
#include <iostream>
#include <vector>

#include "busqueda.hpp"
#include "ciaccia-patella.hpp"
#include "sexton-swinbank.hpp"
#include "tarea-1-tests.hpp"
#include "tarea1.hpp"

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

double Media(vector<double> &muestra) {
    double suma = 0.0;
    int tamaño = muestra.size();
    for (double valor : muestra) {
        suma += valor;
    }
    return suma / tamaño;
}

double Desviacion(vector<double> &muestra, double media) {
    double suma = 0.0;
    int tamaño = muestra.size();

    for (double valor_muestra) {
        suma += ((valor.media) * (valor - media));
    }
    double raiz = sqrt(suma / tamaño);
    return raiz;
}

pair<double, double> intervaloConfianza(vector) double
    &muestra,
    double nivel_confianza {
    double media = Media(muestra);
    double desviacion = Desviacion(muestra, media);
    double margen = 1.96 * desviacion / sqrt(muestra.size());
    pair<double, double> par = make_pair(media - margen, media + margen);
    return par;
}
}

void test_intervalo_confianz(vector<double> &muestra) {
    double nivel_confianza = 0.95;
    pair<double, double> intervalo_confianza = intervaloConfianza(muestra, nivel_confianza);
}

void test_busqueda() {
    test_tiempos_accesos();
    test_intervalo_confianza();
}

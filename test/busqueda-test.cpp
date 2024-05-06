
#include <chrono>
#include <cmath>
#include <vector>

#include "busqueda.hpp"
#include "ciaccia-patella.hpp"
#include "tarea1.hpp"

#include "tests.hpp"

using namespace std;

vector<vector<int>> matriz_accesos(15, std::vector<int>(100));
vector<vector<int>> matriz_tiempos(15, std::vector<int>(100));

void test_tiempos_accesos() {
    for (int i = 0; i < 15; i++) {
        Nodo *arbol = ciaccia_patella(conjuntos[i]);

        for (int m = 0; m < 100; m++) {
            Query query = querys[m];
            Tupla tupla;

            auto inicio = std::chrono::high_resolution_clock::now();
            busqueda(*arbol, query, tupla);
            auto fina = std::chrono::high_resolution_clock::now();
            auto duracion = std::chrono::duration_cast<std::chrono::microseconds>(fina - inicio);

            matriz_tiempos[i][m] = duracion.count();
            matriz_accesos[i][m] = tupla.accesos;
        }
    }
}

double Media(vector<double> &muestra) {
    double suma = 0.0;
    int tamaño = muestra.size();
    for (double valor : muestra) {
        suma += valor;
    }
    return suma / tamaño;
    u() double Desviacion(vector<double> & muestra, double media) {
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

v > oid test_intervalo_confianz(vector<double> &muestra) {
    double nivel_confianza = 0.95;
    pair<double, double> intervalo_confianza = intervaloConfianza(muestra, nivel_confianza);
}

void test_busqueda() {
    test_tiempos_accesos();
    test_intervalo_confianza();
}

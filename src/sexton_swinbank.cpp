
#include <cmath>
#include <limits>
#include <unordered_map>
#include <vector>

#include "minmax.hpp"
#include "sexton_swinbank.hpp"
#include "tarea.hpp"

#define TOMAR_TIEMPO 1

#if TOMAR_TIEMPO

#include <chrono>
#include <iostream>

#define INICIAR_ALGORITMO      \
    auto t = std::chrono::high_resolution_clock::now(); \
    uint tiempos [10]; \
    for (int i = 0; i < 10; i++) { \
        tiempos[i] = 0; \
    }\

#define TERMINAR_PASO(paso) \
    tiempos[paso] += (chrono::high_resolution_clock::now() - t).count(); \
    t = std::chrono::high_resolution_clock::now(); \

#define PRINT_TIEMPOS \
    for (int i = 0; i < 10; i++) { \
        cout << "Tiempo gastado en paso " << i << ": " << tiempos[i] << endl;\
    } \

#else

#define INICIAR_ALGORITMO
#define TERMINAR_PASO(paso)

#endif

using namespace std;

Nodo *sexton_swinbank(Conjunto &c_in) {    
    INICIAR_ALGORITMO
    TERMINAR_PASO(0)

    if (c_in.size() <= B) {
        return crear_nodo(c_in);
    }

    TERMINAR_PASO(1)

    Particion *c_out = crear_clusters(c_in);

    TERMINAR_PASO(2)

    vector<Entry> c;
    unordered_map<Punto, int, PuntoHash> puntos_a_indices;

    for (auto puntos : *c_out)
        c.push_back(output_hoja(puntos));

    delete c_out;

    while (c.size() > B) {

        TERMINAR_PASO(3)

        Conjunto medoides;

        for (uint i = 0; i < c.size(); i++) {
            puntos_a_indices[c[i].p] = i;
            medoides.push_back(c[i].p);
        }

        TERMINAR_PASO(4)

        c_out = crear_clusters(medoides);


        TERMINAR_PASO(5)

        vector<vector<Entry>> c_mra;

        for (Conjunto &puntos : *c_out) {
            vector<Entry> s(puntos.size());

            for (uint i = 0; i < puntos.size(); i++) {
                s[i] = c[puntos_a_indices[puntos[i]]]; // xd
            }

            c_mra.push_back(s);
        }


        TERMINAR_PASO(6)

        c.clear();
        delete c_out;

        for (vector<Entry> &entries : c_mra) {
            c.push_back(output_interno(entries));
        }
        
        TERMINAR_PASO(7)
    }

    Entry entrada = output_interno(c);

    PRINT_TIEMPOS

    return entrada.a;
}

// Retorna el indice del medoide primario de un conjunto de puntos
// y el radio cobertor que este genera sobre el conjunto.
pair<int, double> elegir_medoide(Conjunto const &puntos) {
    vector<double> r_max(puntos.size(), 0);

    // Itera por todos los pared distintos de puntos, seteando por cada uno
    // su maximo radio, así hallando todos los radios cobertores.
    for (uint i = 0; i < puntos.size(); i++) {
        for (uint j = i + 1; j < puntos.size(); j++) {
            double r_cand = distancia(puntos[i], puntos[j]);

            if (r_max[i] < r_cand) r_max[i] = r_cand;
            if (r_max[j] < r_cand) r_max[j] = r_cand;
        }
    }

    // Se itera por todos los radios cobertores para encontrar el mínimo.
    double r_min = numeric_limits<double>::max();
    int i_min = 0;
    for (uint i = 0; i < puntos.size(); i++) {
        if (r_min > r_max[i]) {
            r_min = r_max[i];
            i_min = i;
        }
    }

    // Se retorna el indice del medoide encontrado.
    return {i_min, r_min};
}

Entry output_hoja(Conjunto &c_in) {
    pair<int, double> medoide = elegir_medoide(c_in);
    Nodo *c = crear_nodo(c_in);
    return {c_in[medoide.first], medoide.second, c};
}

Entry output_interno(vector<Entry> const &c_mra) {
    Conjunto c_in(c_mra.size());

    for (uint i = 0; i < c_mra.size(); i++)
        c_in[i] = c_mra[i].p;

    // La función elegir_medoide ya retorna el el radio cobertor del
    // conjunto de puntos por lo que no es necesario re calcularlo.
    pair<int, double> medoide = elegir_medoide(c_in);
    Punto G = c_in[medoide.first];
    double R = medoide.second + c_mra[medoide.first].r;

    Nodo *C = crear_nodo(c_mra);

    return {G, R, C};
}

pair<int, int> elegir_pares_cercanos(Particion const &clusters, vector<int> const &medoides) {
    double d_min = numeric_limits<double>::max();
    pair<int, int> cercanos;

    for (uint i = 0; i < clusters.size(); i++) {
        for (uint j = i + 1; j < clusters.size(); j++) {
            double d_candidata = distancia(clusters[i][medoides[i]], clusters[j][medoides[j]]);

            if (d_candidata < d_min) {
                cercanos.first = i;
                cercanos.second = j;
                d_min = d_candidata;
            }
        }
    }

    return cercanos;
}

Particion *crear_clusters(Conjunto &c_in) {
    Particion c(c_in.size(), vector<Punto>(1));
    vector<int> medoides(c_in.size());

    // Se añaden los conjuntos a c y se definen sus medoides primarios.
    for (uint i = 0; i < c_in.size(); i++) {
        c[i][0] = c_in[i];
        medoides[i] = 0;
    }

    Particion *c_out = new Particion;
    vector<int> medoides_out;

    while (c.size() > 1) {
        pair<int, int> p = elegir_pares_cercanos(c, medoides);

        if (c[p.first].size() + c[p.second].size() < B) {

            // Se añaden los elementos del segundo cluster al primero y luego se elimina.
            c[p.first].insert(c[p.first].end(), c[p.second].begin(), c[p.second].end());
            c.erase(c.begin() + p.second);
            medoides.erase(medoides.begin() + p.second);

            // Finalmente se actualiza el medoide de la union de los conjuntos.
            medoides[p.first] = elegir_medoide(c[p.first]).first;

        } else {
            // Se elimina el cluster más grande y se añade a c_out
            int indice_cluster_mas_grande = c[p.first].size() < c[p.second].size() ? p.second : p.first;

            c_out->push_back(c[indice_cluster_mas_grande]);
            medoides_out.push_back(medoides[indice_cluster_mas_grande]);

            c.erase(c.begin() + indice_cluster_mas_grande);
            medoides.erase(medoides.begin() + indice_cluster_mas_grande);
        }
    }

    Conjunto c_prima;

    if (!c_out->empty()) {
        int mas_cercano = 0;
        double d_min = numeric_limits<double>::max();

        for (uint i = 0; i < c_out->size(); i++) {
            double d_candidata = distancia(c[0][medoides[0]], (*c_out)[i][medoides_out[i]]);

            if (d_candidata < d_min) {
                mas_cercano = i;
                d_min = d_candidata;
            }
        }

        c_prima = std::move((*c_out)[mas_cercano]);
        c_out->erase(c_out->begin() + mas_cercano);
        // medoides_out.erase(medoides_out.begin() + mas_cercano);
    }

    c_prima.insert(c_prima.begin(), c[0].begin(), c[0].end());

    if (c_prima.size() + c[0].size() < B) {
        c_out->push_back(c_prima);
        // medoides_out.push_back(elegir_medoide(c_prima).first);

    } else {
        Conjunto c1;
        Conjunto c2;
        minmax_split(c_prima, c1, c2);
        c_out->push_back(c1);
        c_out->push_back(c2);
        // medoides_out.push_back(elegir_medoide(c1).first);
        // medoides_out.push_back(elegir_medoide(c2).first);
    }

    return c_out;
}

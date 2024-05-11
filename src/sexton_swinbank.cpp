
#include <cmath>
#include <cstddef>
#include <limits>
#include <map>
#include <vector>

#include "sexton_swinbank.hpp"
#include "tarea.hpp"

using namespace std;


Nodo *sexton_swinbank(Conjunto &c_in) {
    if (c_in.size() <= B) {
        Entry entrada = output_hoja(c_in);
        return entrada.a;
    }

    vector<Conjunto> c_out = crear_clusters(c_in);
    vector<Entry> c;
    for (uint i = 0; i < c_out.size(); i++) {
        c.push_back(output_hoja(c_out[i]));
    }
    while (c.size() > B) {
        Conjunto c_in;
        for (uint i = 0; i < c.size(); i++) {
            c_in.push_back(c[i].p);
        }
        c_out = crear_clusters(c_in);
        vector<vector<Entry>> c_mra;
        for (int i = 0; i < c_out.size(); i++) {
            vector<Entry> s;
            for (int j = 0; j < c.size(); j++) {
                Entry entrada;
                entrada.p = c[j].p;
                entrada.a = c[j].a;
                entrada.r = c[j].r;
                for (int k = 0; k < c_out.size(); k++) {
                    if (entrada.p.x == c_out[i][k].x && entrada.p.y == c_out[i][k].y) {
                        s.push_back(entrada);
                        break;
                    }
                }
            }
            c_mra.push_back(s);
        }
        vector<Entry> a;
        c = a;
        for (int i = 0; i < c_mra.size(); i++) {
            c.push_back(output_interno(c_mra[i]));
        }
    }
    Entry entrada1 = output_interno(c);
    return entrada1.a;
}

typedef struct medoide {
    int indice;
    double radio_cobertor;
} Medoide;

Medoide elegir_medoide(Conjunto &puntos) {
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

// Tener en cuenta que en dosConjutnos resultado en la variable Conjunto a debe estar el mayor conjunto
void elegirParesCercanos(Particion c, map<Conjunto, Punto> medoide,
                         map<Conjunto, int> tamaño_conjuntos, dosConjuntos &resultado) {
    int tamaño = c.size();
    double distancia_candidata = numeric_limits<double>::max();

    for (int i = 0; i < tamaño; i++) {
        for (int j = 0; j < tamaño; j++) {

            if (i != j && tamaño_conjuntos[c[i]] >= tamaño_conjuntos[c[j]]) {
                double distancia_actual = distancia(medoide[c[i]], medoide[c[j]]);

                if (distancia_candidata > distancia_actual) {
                    resultado.a = c[i];
                    resultado.b = c[j];
                    resultado.indiceA = i;
                    resultado.indiceB = j;
                    distancia_candidata = distancia_actual;
                }
            }
        }
    }
}

vector<Conjunto> crear_clusters(Conjunto &c_in) {
    int tamaño = c_in.size();
    Particion c_out;
    Particion c;
    map<Conjunto, Punto> medoide;
    map<Conjunto, int> tamaño_conjuntos;

    for (int i = 0; i < tamaño; i++) {
        c[i].push_back(c_in[i]);
        medoide[c[i]] = c_in[i];
        tamaño_conjuntos[c[i]] = 1;
    }

    while (c.size() > 1) {
        dosConjuntos par;
        elegirParesCercanos(c, medoide, tamaño_conjuntos, par);
        if (tamaño_conjuntos[par.a] + tamaño_conjuntos[par.b] <= B) {
            c.erase(c.begin() + par.indiceA);
            c.erase(c.begin() + par.indiceB);
            Conjunto nuevo_conjunto = par.a.insert(par.a.end(), par.b.begin(), par.b.end());
            c.push_back(nuevo_conjunto);
            tamaño_conjuntos.erase(par.a);
            tamaño_conjuntos.erase(par.b);
            medoide.erase(par.a);
            medoide.erase(par.b);
            tamaño_conjuntos[nuevo_conjunto] = nuevo_conjunto.size();
            elegir_medoide(nuevo_conjunto, medoide);
        } else {
            c.erase(c.begin() + par.indiceA);
            c_out.push_back(par.a);
        }
    }

    Conjunto cPrima;
    while (c_out.size() > 0) {
        int tamaño_c = c_out.size();
        double distancia_actual = numeric_limits<double>::max();
        for (int i = 0; tamaño_c; i++) {
            double distancia_candidata = distancia(medoide[c_out[i]], medoide[c[0]]);
            if (distancia_actual > distancia_candidata) {
                distancia_actual = distancia_candidata;
                cPrima = c_out[i];
                c_out.erase(c_out.begin() + i);
            } else {
                cPrima = {};
            }
        }
    }

    Conjunto union1 = c.insert(c.end(), cPrima.begin(), cPrima.end());

    if (c.size() + cPrima.size() <= B) {
        c_out.insert(c_out.end(), union1.begin(), union1.end());
    } else {
        min_max_split();
        Conjunto c1 = par.a;
        Conjunto c2 = par.b;
        c_out.insert(c_out.end(), c1.begin(), c1.end());
        c_out.insert(c_out.end(), c2.begin(), c2.end());
    }

    return c_out;
}

void crear_nodo()

Entry output_hoja(Conjunto &c_in) {
    Medoide medoide = elegir_medoide(c_in);

    Nodo *c = new Nodo;
    for (uint i = 0; i < c_in.size(); i++) {
        añadir_entrada(c, c_in[i]);
    }

    return {c_in[medoide.indice], medoide.radio_cobertor, c};
}

Entry *output_interno(vector<Entry> &c_mra) {
    vector<Punto> c_in;
    Entry resultado;
    Nodo c;
    for (int i = 0; i < c_mra.size(); i++) {
        c_in.push_back(c_mra[i].p);
    }
    double R = 0;
    map<Conjunto, Punto> medoide;
    elegir_medoide(c_in, medoide);
    Punto medoide_primario = medoide[c_in];
    for (int i = 0; i < c_mra.size(); i++) {
        c.entradas[i] = c_mra[i];
        R = max(R, distancia(medoide_primario, c_mra[i].p) + c_mra[i].r);
    }

    resultado.r = R;
    resultado.a = &c;
    resultado.p = medoide_primario;
    return resultado;
}


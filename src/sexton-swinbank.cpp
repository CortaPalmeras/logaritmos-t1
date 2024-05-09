
#include <cmath>
#include <cstddef>
#include <limits>
#include <map>
#include <random>
#include <vector>

#include "sexton-swinbank.hpp"
#include "tarea1.hpp"

using namespace std;

typedef struct {
    Conjunto a;
    Conjunto b;
    int indiceA;
    int indiceB;
} dosConjuntos;

void elegir_medoide(Conjunto c_in, map<Conjunto, Punto> &medoide) {
    int tamaño = c_in.size();
    map<Punto, int> candidatos;

    for (int i = 0; i < tamaño; i++) {
        double distancia_maxima = 0;

        for (int j = 0; j < tamaño; j++) {
            if (i != j) {
                double distancia_actual = distancia(c_in[i], c_in[j]);
                if (distancia_actual > distancia_maxima) {
                    distancia_maxima = distancia_actual;
                }
            }
        }
        candidatos[c_in[i]] = distancia_maxima;
    }
    Punto candidato;
    for (int i = 0; i < tamaño; i++) {
        int candidato_anterior;
        int candidato_actual = candidatos[c_in[i]];
        if (i == 0) {
            candidato_anterior = candidatos[c_in[i]];
            candidato = c_in[0];
            continue;
        } else {
            if (candidato_actual > candidato_anterior) {
                candidato = c_in[i];
            }
        }
    }
    medoide[c_in] = candidato;
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
        dosConjuntos par = MinMaxSplit(union1);
        Conjunto c1 = par.a;
        Conjunto c2 = par.b;
        c_out.insert(c_out.end(), c1.begin(), c1.end());
        c_out.insert(c_out.end(), c2.begin(), c2.end());
    }

    return c_out;
}

Entry OutPutHoja(Conjunto &c_in) {
    Entry resultado;
    Nodo c;
    map<Conjunto, Punto> medoide;
    elegir_medoide(c_in, medoide);
    Punto medoide_primario = medoide[c_in];
    resultado.p = medoide_primario;
    double r = 0;
    for (int i = 0; i < c_in.size(); i++) {
        Entry entrada;
        entrada.p = c_in[i];
        entrada.a = NULL;
        entrada.r = 0;
        c.entradas[i] = entrada;
        r = max(r, distancia(medoide_primario, entrada.p));
    }
    resultado.r = r;
    resultado.a = &c;
}

Entry OutPutInterno(vector<Entry> c_mra) {
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

Nodo sexton_swinbank(Conjunto c_in) {
    if (c_in.size() <= B) {
        Entry entrada = OutPutHoja(c_in);
        return *(entrada.a);
    }
    vector<Conjunto> c_out = crear_clusters(c_in);
    vector<Entry> c;
    for (int i = 0; i < c_out.size(); i++) {
        c.push_back(OutPutHoja(c_out[i]));
    }
    while (c.size() > B) {
        Conjunto c_in;
        for (int i = 0; i < c.size(); i++) {
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
            c.push_back(OutPutInterno(c_mra[i]));
        }
    }
    Entry entrada1 = OutPutInterno(c);
    return *entrada1.a
}


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

// Función potencialmente inutil, la escribi y despues me di cuenta de que no servía.
double radio_cobertor(Conjunto &a, Conjunto &b, int indice) {
    double r_max = numeric_limits<double>::min();

    for (int i = 0; i < a.size(); i++) {
        if (i != indice) {
            double r_candidato = distancia(a[indice], a[i]);
            r_max = r_max > r_candidato ? r_max : r_candidato;
        }
    }

    for (int i = 0; i < b.size(); i++) {
        if (i != indice) {
            double r_candidato = distancia(a[indice], b[i]);
            r_max = r_max > r_candidato ? r_max : r_candidato;
        }
    }

    return r_max;
}

typedef struct dll {
    int indice = 0;
    dll *siguiente = NULL;
    dll *previo = NULL;
} doubly_linked_list;

void MinMaxSplit(Conjunto &puntos, Conjunto **out1, Conjunto **out2) {
    double r_min = numeric_limits<double>::max();

    // calcular distancias entre todos los puntos,
    // para que no se calculen más de una vez en la función.
    vector< vector<double> > d(puntos.size(), vector<double>(puntos.size()));

    for (int i = 0; i < puntos.size(); i++) {
        d[i][i] = 0;

        for (int j = i + 1; j < puntos.size(); j++) {
            double dist = distancia(puntos[i], puntos[j]);
            d[i][j] = dist;
            d[j][i] = dist;
        }
    }


    // para cada par de puntos:
    for (int i = 0; i < puntos.size(); i++) {
        for (int j = i + 1; j < puntos.size(); j++) {

            // se crean dos listas doblemente enlazadas de puntos ordenados 
            // en relación a su distancia con los dos puntos elegidos.
            doubly_linked_list cola_distancias1[puntos.size()];
            doubly_linked_list cola_distancias2[puntos.size()];

            doubly_linked_list *primero1 = cola_distancias1;
            doubly_linked_list *primero2 = cola_distancias2;

            // Se van añadiendo los indices de los puntos uno por uno a las colas
            for (int n = 0; n < puntos.size(); n++) {
                if (n == i || n == j) continue;

                cola_distancias1[n].indice = n;
                cola_distancias2[n].indice = n;

                bool falta1 = true;
                bool falta2 = true;

                // Caso en el quel la distancia con un punto 
                // sea menor que la del primer punto de la cola:
                if (d[i][n] < d[i][primero1->indice]) {
                    primero1->previo = cola_distancias1 + n;
                    cola_distancias1[n].siguiente = primero1;
                    primero1 = cola_distancias1 + n;
                    falta1 = false;
                }

                if (d[j][n] < d[j][primero2->indice]) {
                    primero2->previo = cola_distancias1 + n;
                    cola_distancias2[n].siguiente = primero1;
                    primero2 = cola_distancias1 + n;
                    falta2 = false;
                }
                
                doubly_linked_list *nodo_actual = primero1->siguiente;

                // se avanza por las colas hasta encontrar donde debe ir el punto n.
                while (falta1) {
                    if (d[i][n] < d[i][nodo_actual->indice]) {
                        nodo_actual->previo->siguiente = cola_distancias1 + n;
                        nodo_actual->previo = cola_distancias1 + n;
                        cola_distancias1[n].siguiente = nodo_actual;
                        falta1 = false;

                    } else if (nodo_actual->siguiente == NULL) {
                        nodo_actual->siguiente = cola_distancias1 + n;
                        nodo_actual = cola_distancias1 + n;
                        falta1 = false;

                    } else {
                        nodo_actual = nodo_actual->siguiente;
                    }
                }

                nodo_actual = primero2->siguiente;

                while (true) {
                    if (d[j][n] < d[j][nodo_actual->indice]) {
                        nodo_actual->previo->siguiente = cola_distancias2 + n;
                        nodo_actual->previo = cola_distancias2 + n;
                        cola_distancias2[n].siguiente = nodo_actual;
                        falta2 = false;

                    } else if (nodo_actual->siguiente == NULL) {
                        nodo_actual->siguiente = cola_distancias2 + n;
                        nodo_actual = cola_distancias2 + n;
                        falta2 = false;

                    } else {
                        nodo_actual = nodo_actual->siguiente;
                    }
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

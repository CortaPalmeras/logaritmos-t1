
#include <cmath>
#include <limits>
#include <map>
#include <random>
#include <vector>

#include "tarea1.hpp"
#include "ciaccia-patella.hpp"

using namespace std;


// Algoritmo de Ciaccia - Patella
Nodo *ciaccia_patella(Conjunto const &p_in) {
paso_1:
    if (p_in.size() < B) {
        Nodo *T = crear_nodo();
        for (auto p : p_in) {
            anadir_entrada(T, p);
        }
        return T;
    }

paso_2:
    int k = p_in.size() / B;
    k = k < B ? k : B;
    Conjunto *F = random_sample(p_in, k);

paso_3:
    Particion *particion = crear_particion(p_in, *F);

paso_4:
    for (int i = 0, fin = particion->size(); i < fin;) {
        if ((*particion)[i].size() < b) {
            // Se guardan los puntos que se deben redistribuir.
            Conjunto tmp = (*particion)[i];

            // Se quita p_fi de F y se elimina F_i.
            F->erase(F->begin() + i);
            particion->erase(particion->begin() + i);

            // Se redistribuyen los puntos.
            asignar_puntos_a_samples(tmp, *F, *particion);

            fin--;
        } else {
            i++;
        }
    }

paso_5:
    if (F->size() <= 1) {
        delete F;
        delete particion;
        goto paso_2;
    }

paso_6:
    vector<Nodo *> arboles(particion->size());
    for (int i = 0; i < particion->size(); i++) {
        arboles[i] = ciaccia_patella((*particion)[i]);
    }
    delete particion;

paso_7:
    for (int i = 0, fin = arboles.size(); i < fin;) {
        Nodo *arbol = arboles[i];

        if (arbol->size < b) {
            // Se elimina la raiz del conjunto de arboles
            // y se elimina el punto p del conjunto F.
            F->erase(F->begin() + i);
            arboles.erase(arboles.begin() + i);

            // Se incrementa el tamaño de F y el conjunto de arboles.
            int F_size = F->size();
            F->resize(F_size + arbol->size);

            int a_size = arboles.size();
            arboles.resize(a_size + arbol->size);

            // ---- ACORDARSE DE BORRAR ESTO ----
            // Me di cuenta de que la partición ya no es necesario
            // pero no quiero borrar el codigo todavía por si acaso.
            // particion->erase(particion->begin() + i);
            // int p_size = particion->size();
            // particion->resize(p_size + arbol->size);

            // Se añaden los sub arboles al cunjunto de arboles
            // y los puntos pertinentes a F.
            for (int j = 0; j < arbol->size; j++) {
                (*F)[F_size + j] = arbol->entradas[j].p;
                arboles[a_size + j] = arbol->entradas[j].a;

                // Esto tampoco es necesario
                // extraer_hojas(*arbol, (*particion)[p_size + j]);
            }

            // Se libera la memoria del nodo raiz.
            delete arbol;

            fin--;
        } else {
            i++;
        }
    }

paso_8:
    int h_minima = numeric_limits<int>::max();
    vector<int> alturas(arboles.size());

    for (int i = 0; i < arboles.size(); i++) {
        alturas[i] = altura_arbol(*arboles[i]);
        h_minima = h_minima < alturas[i] ? h_minima : alturas[i];
    }

paso_9:
    map<Punto, Nodo *> balanceamiento;
    Conjunto F_balanceamiento;
    for (int i = 0; i < arboles.size(); i++) {
        if (alturas[i] == h_minima) {
            balanceamiento[(*F)[i]] = arboles[i];
            F_balanceamiento.push_back((*F)[i]);
        } else {
            extraer_subarboles(arboles[i], balanceamiento, F_balanceamiento, alturas[i] - h_minima);
        }
    }
    delete F;

paso_10:
    Nodo *T_superior = ciaccia_patella(F_balanceamiento);

paso_11:
    unir_subarboles(T_superior, balanceamiento);

paso_12:
    asignar_distancias_arbol(*T_superior);

paso_13:
    return T_superior;
}




Conjunto *random_sample(Conjunto const &puntos, int k) {
    int n = puntos.size();
    vector<int> indices(n);
    Conjunto *F = new vector<Punto>;

    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }

    for (int i = 0; i < k; i++) {
        int j = dist_int_0_inf(rdg) % (n - i) + i;
        F->push_back(puntos[indices[j]]);
        indices[j] = indices[i];
    }

    return F;
}

Particion *crear_particion(Conjunto const &puntos, Conjunto const &samples) {
    Particion *particion = new Particion(samples.size());
    asignar_puntos_a_samples(puntos, samples, *particion);
    return particion;
}

void asignar_puntos_a_samples(Conjunto const &puntos, Conjunto const &samples, Particion &particion) {
    for (int i = 0; i < puntos.size(); i++) {
        int indice_sample_mas_cercano;
        double distancia_minima = numeric_limits<double>::max();

        for (int j = 0; j < samples.size(); j++) {
            double distancia_candidata = distancia(puntos[i], samples[j]);

            if (distancia_minima > distancia_candidata) {
                distancia_minima = distancia_candidata;
                indice_sample_mas_cercano = j;
            }
        }

        particion[indice_sample_mas_cercano].push_back(puntos[i]);
    }
}

void extraer_subarboles(Nodo *arbol, map<Punto, Nodo *> &raices, Conjunto &puntos, int dif) {
    if (dif == 1) {
        for (int i = 0; i < arbol->size; i++) {
            raices[arbol->entradas[i].p] = arbol->entradas[i].a;
            puntos.push_back(arbol->entradas[i].p);
        }
    } else {
        for (int i = 0; i < arbol->size; i++) {
            extraer_subarboles(arbol->entradas[i].a, raices, puntos, dif - 1);
        }
    }
    delete arbol;
}

void unir_subarboles(Nodo *arbol, map<Punto, Nodo *> &subarboles) {
    if (arbol->entradas[0].a == NULL) {
        for (int i = 0; i < arbol->size; i++) {
            arbol->entradas[i].a = subarboles[arbol->entradas[i].p];
        }
    } else {
        for (int i = 0; i < arbol->size; i++) {
            unir_subarboles(arbol->entradas[i].a, subarboles);
        }
    }
}

void asignar_distancias_arbol(Nodo &arbol) {
    if (arbol.entradas[0].a != NULL) {
        for (int i = 0; i < arbol.size; i++) {
            asignar_distancias_entrada(arbol.entradas[i]);
        }
    }
}

void asignar_distancias_entrada(Entry &entrada) {
    Nodo &nodo = *entrada.a;

    if (nodo.entradas[0].a == NULL) {
        double d_maxima = numeric_limits<int>::min();
        punto punto_lejano;

        for (int i = 0; i < nodo.size; i++) {
            double d_candidata = distancia(entrada.p, nodo.entradas[i].p);
            if (d_maxima < d_candidata) {
                d_maxima = d_candidata;
            }
        }

        entrada.r = d_maxima;

    } else {
        for (int i = 0; i < nodo.size; i++) {
            asignar_distancias_entrada(nodo.entradas[i]);

            if (nodo.entradas[i].p == entrada.p) {
                entrada.r = nodo.entradas[i].r;
            }
        }
    }
}

// Función inutil :)
// void extraer_hojas(Nodo const &arbol, Conjunto &out) {
//     for (auto entrada : arbol.entradas) {
//         if (entrada.a == NULL) {
//             out.push_back(entrada.p);
//         } else {
//             extraer_hojas(*entrada.a, out);
//         }
//     }
// }
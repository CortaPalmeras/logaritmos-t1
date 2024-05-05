
#include <cmath>
#include <iostream>
#include <limits>
#include <map>
#include <random>
#include <vector>

using namespace std;

random_device dev;
mt19937 rdg(dev());
uniform_real_distribution<double> dist_0_1(0.0, 1.0);
uniform_real_distribution<double> dist_0_inf(0.0, numeric_limits<double>::max());
uniform_int_distribution<int> dist_int_0_inf(0.0, numeric_limits<int>::max());

struct punto;
struct query;
struct entry;
struct nodo;

typedef struct punto {
    double x;
    double y;
} Punto;

typedef struct query {
    Punto p;
    double r;
} Query;

typedef struct entry {
    Punto p;
    double r;
    struct nodo *a;
} Entry;

const int B = (4096 - sizeof(int)) / sizeof(Entry);
const int b = B / 2;

typedef struct nodo {
    Entry entradas[B];
    int size;
} Nodo;

typedef vector<punto> Conjunto;
typedef vector<Conjunto> Particion;

bool operator==(Punto const &a, Punto const &b) {
    return a.x == b.x && a.y == b.y;
}

nodo *crear_nodo() {
    Nodo *n = new Nodo;
    n->size = 0;
    return n;
}

void anadir_entrada(Nodo *n, Punto p, Nodo *a) {
    if (n->size >= B) {
        cout << "nodo sobrepasa el tamaño máximo en 'anadir_entrada'" << endl;
        exit(1);
    }
    int s = n->size;
    n->entradas[s].p = p;
    n->entradas[s].a = a;
    n->entradas[s].r = 0;
    n->size++;
}

void eliminar_arbol(Nodo *arbol) {
    for (int i = 0; i < arbol->size; i++) {
        eliminar_arbol(arbol->entradas[i].a);
        delete arbol;
    }
}

double distancia(Punto const &a, Punto const &b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
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

void asignar_distancias_arbol(Nodo &arbol) {
    if (arbol.entradas[0].a != NULL) {
        for (int i = 0; i < arbol.size; i++) {
            asignar_distancias_entrada(arbol.entradas[i]);
        }
    }
}

void extraer_hojas(Nodo const &arbol, Conjunto &out) {
    for (auto entrada : arbol.entradas) {
        if (entrada.a == NULL) {
            out.push_back(entrada.p);
        } else {
            extraer_hojas(*entrada.a, out);
        }
    }
}

int altura_arbol(Nodo const &arbol) {
    if (arbol.entradas[0].a == NULL) {
        return 1;
    } else {
        return altura_arbol(*arbol.entradas[0].a) + 1;
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

vector<Punto> *random_sample(vector<Punto> const &puntos, int k) {
    int n = puntos.size();
    vector<int> indices(n);
    vector<Punto> *F = new vector<Punto>;

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

Particion *crear_particion(Conjunto const &puntos, Conjunto const &samples) {
    Particion *particion = new Particion(samples.size());
    asignar_puntos_a_samples(puntos, samples, *particion);
    return particion;
}

Nodo *ciaccia_patella(Conjunto const &p_in) {
paso_1:
    if (p_in.size() < B) {
        Nodo *T = crear_nodo();
        for (auto p : p_in) {
            anadir_entrada(T, p, NULL);
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

        if (arbol->size < b) { // Se elimina la raiz del conjunto de arboles
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

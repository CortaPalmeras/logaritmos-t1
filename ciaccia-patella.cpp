#include <cmath>
#include <limits>
#include <list>
#include <random>
#include <vector>

using namespace std;

// Creo que el B se calcula a partir de Bsizeof(entry)=4096 dado que se dice en
// el enunciado que 4096 equivale a un bloque y el acceso a un bloque equivale
// al acceso a un nodo

random_device dev;
mt19937 generador(dev());
uniform_real_distribution<double> dist_0_1(0.0, 1.0);
uniform_real_distribution<double> dist_0_inf(0.0, numeric_limits<double>::max());
uniform_int_distribution<int> dist_0_inf(0.0, numeric_limits<int>::max());

const int B = 4096;
const int b = (0.5 * B);

typedef struct {
    double x;
    double y;
} Punto;

typedef struct {
    Punto punto;
    double radio;
} Query;

typedef struct Nodo {
    Punto punto;
    double radio;
    vector<Nodo> direcciones;
} Nodo;

int distancia(Punto &a, Punto &b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

// falta colocar la cantidad de accesos a disco
vector<Nodo> Busqueda(Nodo &arbol, Query &consulta) {
    vector<Nodo> resultado;
    for (auto it : arbol.direcciones) {
        if (it.direcciones.empty()) {
            if (distancia(it.punto, consulta.punto) < consulta.radio) {
                resultado.push_back(it);
            }
        } else {
            if (distancia((it).punto, consulta.punto) <= ((it).radio + consulta.radio)) {
                for (auto it1 : it.direcciones) {
                    vector<Nodo> busqueda = Busqueda(it1, consulta);
                    resultado.insert(resultado.end(), busqueda.begin(), busqueda.end());
                }
            }
        }
    }
    return resultado;
}

// Se ocupa en el paso 3 y en el 4.2
void AsignarSamples(vector<Nodo> &samples, vector<Nodo> &puntos, int cantidad_puntos, int k) {
    for (int i = 0; i < cantidad_puntos; i++) {
        Nodo candidato;
        double distancia_minima = numeric_limits<double>::max();

        for (int m = 0; m < k; m++) {
            double distancia_candidata = distancia(samples[m].punto, puntos[i].punto);

            if (distancia_minima > distancia_candidata) {
                distancia_minima = distancia_candidata;
                candidato = samples[m];
            }
        }
        candidato.direcciones.push_back(puntos[i]);
    }
}

vector<Nodo> random_subset(vector<Nodo> &puntos, int k) {
    int n = puntos.size();
    vector<int> indices(puntos.size());
    uniform_real_distribution<int> dist_int()

    for (int i = 0; i < n; i++) {
        indices[i] = i;
    }

    // Shuffle the vector using Fisher-Yates algorithm
    for (int i = 0; i < k; i++) {
        int j = % (i + 1);
        swap(indices[i], indices[j]);
    }
}

Nodo MetodoCiaciaPatella(vector<Nodo> &puntos) {
    // Paso 1:Demasiado vago el enunciado, confirmar con el profe.
    int cantidad_puntos = puntos.size();
    if (cantidad_puntos <= B) {
        Nodo &arbol = puntos[0];
        for (int i = 0; i < puntos.size(); i++) {
            (arbol.direcciones)[i] = (puntos)[i + 1];
        }
        return arbol;
    }

    int k = min(B, (cantidad_puntos / b));
    int PrimeraVez = 0;
    vector<Nodo> samples;

    while (PrimeraVez == 0 || samples.size() == 1) {
        PrimeraVez = 1;
        // Paso 2:No cacho si hay que descartar aquellos numeros aleatorios que han
        // salido antes o no.  De momento se asume que se descartan.
        // for (int i = 0; i < k; i++) {
        //     cantidad_puntos = puntos.size();
        //     uniform_int_distribution<int> distribucion(0, cantidad_puntos);
        //     int aleatorio = distribucion(generador);
        //     samples.push_back((puntos)[aleatorio]);
        //     puntos.erase(puntos.begin() + aleatorio);
        // }

        // Paso 3: Estoy suponiendo que los samples son las raices y la idea es que
        // los puntos que no son samples se unan a los samples mas cercanos,
        for (int i = 0; i < cantidad_puntos; i++) {
            Nodo candidato;
            double distancia_minima = numeric_limits<double>::max();
            for (int m = 0; m < k; m++) {
                double distancia_candidata = distancia(samples[m].punto, (puntos)[i].punto);
                if (distancia_minima > distancia_candidata) {
                    distancia_minima = distancia_candidata;
                    candidato = samples[m];
                }
            }
            candidato.direcciones.push_back((puntos)[i]);
        }

        // Paso 4:
        for (int i = 0; i < k; i++) {
            if (samples[i].direcciones.size() + 1 < b) {
                Nodo temporal = samples[i];
                samples.erase(samples.begin() + i);
                AsignarSamples(samples, puntos, cantidad_puntos, k);
            }
        }
    }

    // Paso 7:
    for (int i = 0; i < k; i++) {
    }
}

double gen_coordenada() {
    return dist_0_1(generador);
}

double gen_radio() {
    return dist_0_inf(generador);
}

vector<Nodo> crear_conjunto_nodos(int cantidad) {
    vector<Nodo> resultado(cantidad);

    for (int i = 0; i < cantidad; i++) {
        resultado[i].punto.x = gen_coordenada();
        resultado[i].punto.y = gen_coordenada();
        resultado[i].radio = gen_radio();
    }

    return resultado;
}

vector<Query> crear_conjunto_query() {
    int n_queries;
    vector<Query> conjunto_queries(n_queries);

    for (int i = 0; i < n_queries; i++) {
        conjunto_queries[i].punto.x = gen_coordenada();
        conjunto_queries[i].punto.y = gen_coordenada();
        conjunto_queries[i].radio = 0.02;
    }

    return conjunto_queries;
}
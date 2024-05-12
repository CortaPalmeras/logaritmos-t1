// Este archivo contiene declaraciones que se utilizan en general a lo largo de la tarea.

#ifndef TAREA_1
#define TAREA_1

#include <cmath>
#include <cstdlib>
#include <vector>

#define GLOBAL_DEFAULT_RANDOM_SEED 37

typedef struct punto {
    double x;
    double y;
} Punto;

typedef struct query {
    Punto p;
    double r;
} Query;

struct nodo;
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

typedef std::vector<punto> Conjunto;
typedef std::vector<Conjunto> Particion;

inline bool operator==(Punto const &a, Punto const &b) {
    return std::abs(a.x - b.x) < 0.00001 && std::abs(a.y - b.y) < 0.00001;
}

inline bool operator<(Punto const &a, Punto const &b) {
    return a.x < b.x;
}

inline double distancia(Punto const &a, Punto const &b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

struct PuntoHash {
    size_t operator()(Punto const &p) const {
        return std::hash<double>()(p.x) ^ (std::hash<double>()(p.y) << 1);
    }
};

Nodo *crear_nodo();
Nodo *crear_nodo(Conjunto const &puntos);
Nodo *crear_nodo(std::vector<Entry> const &entradas);

void añadir_entrada(Nodo *n, Entry e);
void añadir_entrada(Nodo *n, Punto p);

int altura_arbol(Nodo const &arbol);

void eliminar_arbol(Nodo *arbol);

double random_double_0_a_1();
double random_double_positivo();
int random_int_positivo();

void reiniciar_rng();
void reiniciar_rng(int seed);

#endif

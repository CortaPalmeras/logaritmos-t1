// Este archivo contiene declaraciones que se utilizan en general a lo largo de la tarea.

#ifndef TAREA_1
#define TAREA_1

#include <cmath>
#include <limits>
#include <random>
#include <vector>

struct punto;
struct query;
struct entry;
struct nodo;

#define GLOBAL_RANDOM_SEED 37

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

typedef std::vector<punto> Conjunto;
typedef std::vector<Conjunto> Particion;

inline bool operator==(Punto const &a, Punto const &b) {
    return a.x == b.x && a.y == b.y;
}

inline double distancia(Punto const &a, Punto const &b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

Nodo *crear_nodo();

void anadir_entrada(Nodo *n, Punto p);

int altura_arbol(Nodo const &arbol);

void eliminar_arbol(Nodo *arbol);

inline double random_double();
inline double random_double_0_a_1();
inline double random_double_positivo();
inline int random_int_positivo();

#endif

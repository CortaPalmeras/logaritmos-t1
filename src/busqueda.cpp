
#include <cmath>

#include "tarea1.hpp"
#include "busqueda.hpp"

void busqueda(Nodo &arbol, Query &consulta, tupla &Tupla) {
    if(arbol.entradas[0].a==NULL){
        for (int i = 0; i < arbol.size; i++) {
            if(distancia(arbol.entradas[i].p, consulta.p) <= consulta.r){
                Tupla.conjunto.push_back(arbol.entradas[i].p);
            } 
            Tupla.accesos++;
        }

    } else {
        for (int i = 0; i < arbol.size; i++) {
            if(distancia(arbol.entradas[i].p, consulta.p)  <= arbol.entradas[i].r + consulta.r){
                busqueda(*(arbol.entradas[i].a), consulta, Tupla);
            }
            Tupla.accesos++;
        }
    }
}

Conjunto *crear_conjunto_puntos(int cantidad) {
    Conjunto *resultado = new Conjunto;

    for (int i = 0; i < cantidad; i++) {
        Punto punto;
        punto.x = gen_coordenada();
        punto.y = gen_coordenada();
        resultado->push_back(punto);
    }
    return resultado;
}

ConjuntoQ *crear_conjunto_query() {
    int n_queries=100;
    ConjuntoQ *conjunto_queries=new ConjuntoQ;

    for (int i = 0; i < n_queries; i++) {
        Punto punto;
        Query query;
        punto.x = gen_coordenada();
        punto.y = gen_coordenada();        
        query.p=punto;
        conjunto_queries->push_back(query);
        (*conjunto_queries)[i].r = 0.02;
    }
    return conjunto_queries;
}

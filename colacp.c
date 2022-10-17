#include <stdio.h>
#include <stdlib.h>
typedef void * TClave;
typedef void * TValor;

typedef struct entrada {
    TClave clave;
    TValor valor;
} * TEntrada;

typedef void* TClave;
typedef void* TValor;

typedef struct TNodo {
    TEntrada entrada;
    struct TNodo * padre;
    struct TNodo * hijo_izquierdo;
    struct TNodo * hijo_derecho;
} * TNodo;

typedef struct cola_con_prioridad {
    int cantidad_elementos;
    TNodo raiz;
    int (*comparador)(TEntrada, TEntrada);
} * TColaCP;

TColaCP crearColaCp(int (*f)(TEntrada, TEntrada));
int cpInsertar(TColaCP cola, TEntrada entr);
TEntrada cpEliminar(TColaCP cola);
int cpCantidad(TColaCP cola);
void cpDestruir(TColaCP cola, void (*fEliminar)(TEntrada));

TColaCP crearColaCp(int (*f)(TEntrada, TEntrada)){
    TColaCP cola = (TColaCP)malloc(sizeof(struct cola_con_prioridad));
    TNodo raiz = (TNodo)malloc(sizeof(struct TNodo));
    cola->raiz = raiz;
    cola->cantidad_elementos = 0;
    cola->comparador = f;
    return cola;
}
int cpInsertar(TColaCP cola, TEntrada entr){
    if(cpCantidad(cola) == 0){
        cola->raiz->entrada = entr;
        cola->cantidad_elementos++;
        return 1;
    }else{
        int inserte = 0;
        TNodo actual = cola->raiz;
        while(!inserte){
            int comp = cola->comparador(entr, actual->entrada);
            if(comp == 0)
                return 0;
            else if (comp == -1)
                actual = actual->hijo_derecho;
            else
                actual = actual->hijo_izquierdo;
            if(actual->entrada == NULL){
                actual->entrada = entr;
                cola->cantidad_elementos++;
                return 1;
            }
        }
        return 0;
    }
}
TEntrada cpEliminar(TColaCP cola){
    TEntrada ret = cola->raiz->entrada;
    TNodo aux = cola->raiz;
    if(cola->cantidad_elementos == 1){
        cola->raiz = NULL;
        cola->cantidad_elementos--;
    }else{
        cola->raiz = cola->raiz->hijo_derecho;
        cola->cantidad_elementos--;
    }
    free(aux);
    return ret;
}
int cpCantidad(TColaCP cola){
    return cola->cantidad_elementos;
}
void cpDestruir(TColaCP cola, void (*fEliminar)(TEntrada)){
    while(cola->cantidad_elementos != 0){
        fEliminar(cola->raiz->entrada);
        cola->raiz = cola->raiz->hijo_derecho;
    }
}
int main()
{
    printf("Hello world!\n");
    return 0;
}

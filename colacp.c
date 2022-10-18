#include <stdio.h>
#include <stdlib.h>

#define FALSE 0;
#define TRUE 1;
#define CCP_NO_INI 2;
#define POS_NULA NULL;
#define ELE_NULO NULL;

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
    cola->raiz->padre = POS_NULA;
    cola->cantidad_elementos = 0;
    cola->comparador = f;
    return cola;
}
void insertarRec(TColaCP cola, TNodo actual){
    cola->raiz = actual;
}

int cpInsertar(TColaCP cola, TEntrada entr){
    if(cpCantidad(cola) == 0){
        cola->raiz->entrada = entr;
        cola->cantidad_elementos++;
        return 1;
    }else{
        //insertar en la ult posicion
        //ir chequeando si la prioridad del padre es mayor a la actual
        TNodo nuevo = malloc(sizeof(struct TNodo));
        nuevo->entrada = entr;
        insertarRec(cola, nuevo);
        while((nuevo->padre != NULL) && cola->comparador(nuevo->padre->entrada, nuevo->entrada) == 1){
            //swap de nuevo y padre
            TNodo padreAnterior = nuevo->padre;
            nuevo->padre = nuevo->padre->padre; //es el ancestro
            padreAnterior->padre = nuevo;
            if(nuevo->padre->hijo_derecho == nuevo){
                //es el hd
                nuevo->padre->hijo_derecho = nuevo;
                nuevo->hijo_derecho = padreAnterior;
                nuevo->hijo_izquierdo = padreAnterior->hijo_izquierdo;
            }else{
                //es el hi
                nuevo->padre->hijo_izquierdo = nuevo;
                nuevo->hijo_izquierdo = padreAnterior;
                nuevo->hijo_derecho = padreAnterior->hijo_derecho;
            }
            padreAnterior->hijo_derecho = ELE_NULO;
            padreAnterior->hijo_izquierdo = ELE_NULO;
        }
        return 1;
    }
    return 0;
}

TEntrada cpEliminar(TColaCP cola){
    TEntrada ret = cola->raiz->entrada;
    TNodo aux = cola->raiz;
    if(cola->cantidad_elementos == 1){
        cola->raiz = POS_NULA;
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
        //empezar por hi de la raiz hasta q no tenga hijos
        fEliminar(cola->raiz->entrada);
        cola->raiz = cola->raiz->hijo_derecho;
    }
}


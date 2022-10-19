#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
void nodoEnPosicion(TColaCP cola, TNodo actual, TNodo nuevo, int nivel){
    //actual esta en el hijo mas a la izquierda del ultimo nivel
    int encontre = FALSE;
    while(!encontre){
        if(actual->padre->hijo_derecho == NULL){
            encontre = TRUE;
            actual->padre->hijo_derecho = nuevo;
        }
    }
}

int cpInsertar(TColaCP cola, TEntrada entr){
    if(cpCantidad(cola) == 0){
        cola->raiz->entrada = entr;
        cola->cantidad_elementos++;
        cola->raiz->hijo_derecho = POS_NULA;
        cola->raiz->hijo_izquierdo = POS_NULA;
        return TRUE;
    }else{
        //insertar en la ult posicion
        //ir chequeando si la prioridad del padre es mayor a la actual
        TNodo nuevo = malloc(sizeof(struct TNodo));
        nuevo->entrada = entr;
        int nivel = (int)(log(cola->cantidad_elementos)/log(2));
        int lleno = (log(cola->cantidad_elementos+1)/log(2))/nivel == 1;
        //si lleno es true, debo insertar en un nuevo nivel

        TNodo actual = cola->raiz;
        for(int i = 0; i < nivel; i++)
            actual = actual->hijo_izquierdo;

        if(lleno){
            actual->hijo_izquierdo = nuevo;
            cola->cantidad_elementos++;
        }else{
            nodoEnPosicion(cola, cola->raiz,nuevo, nivel); //supongo q funciona
            cola->cantidad_elementos++;
        }

        while((nuevo->padre != NULL) && cola->comparador(nuevo->padre->entrada, nuevo->entrada) == 1){
            //swap de nuevo y padre
            TNodo padreAnterior = nuevo->padre;
            nuevo->padre = nuevo->padre->padre;
            padreAnterior->padre = nuevo;
            TNodo hi = nuevo->hijo_izquierdo;
            TNodo hd = nuevo->hijo_derecho;
            if(nuevo->padre->hijo_izquierdo == padreAnterior){
                //es el hd
                nuevo->padre->hijo_izquierdo = nuevo;
                nuevo->hijo_izquierdo = padreAnterior;
                nuevo->hijo_derecho = padreAnterior->hijo_derecho;
                padreAnterior->hijo_izquierdo = hi;
                padreAnterior->hijo_derecho = hd;
            }else{
                //es el hi
                nuevo->padre->hijo_derecho = nuevo;
                nuevo->hijo_derecho = padreAnterior;
                nuevo->hijo_izquierdo = padreAnterior->hijo_izquierdo;
                padreAnterior->hijo_izquierdo = hi;
                padreAnterior->hijo_derecho = hd;
            }
        }
        return TRUE;
    }
    return FALSE;
}
TNodo buscarUltimo(){
    return NULL;
}
TEntrada cpEliminar(TColaCP cola){
    TEntrada ret = cola->raiz->entrada;
    TNodo aux = cola->raiz;
    if(cola->cantidad_elementos == 0)
        return ELE_NULO;

    if(cola->cantidad_elementos == 1){
        cola->raiz = POS_NULA;
        cola->cantidad_elementos--;
    }else{
        TNodo ult = buscarUltimo(); //supongo q anda
        ult->hijo_derecho = cola->raiz->hijo_derecho;
        ult->hijo_izquierdo = cola->raiz->hijo_izquierdo;
        cola->raiz = ult;
        int huboSwap = TRUE;
        while(huboSwap){
            huboSwap = FALSE;
            int prioridadMin = cola->comparador(ult->hijo_izquierdo->entrada, ult->hijo_derecho->entrada);
            if(prioridadMin == -1){
                if(cola->comparador(ult->entrada, ult->hijo_izquierdo->entrada) == 1){
                    //swap
                    huboSwap = TRUE;
                    ult = ult->hijo_izquierdo;
                }
            }else{
                if(cola->comparador(ult->entrada, ult->hijo_derecho->entrada) == 1){
                    //swap
                    huboSwap = TRUE;
                    ult = ult->hijo_derecho;
                }
            }
        }
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


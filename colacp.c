#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FALSE 0
#define TRUE 1
#define CCP_NO_INI 2
#define POS_NULA NULL
#define ELE_NULO NULL

typedef void * TClave;
typedef void * TValor;

typedef struct entrada {
    TClave clave;
    TValor valor;
} * TEntrada;

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
    cola->raiz = ELE_NULO;
    cola->cantidad_elementos = 0;
    cola->comparador = f;
    return cola;
}

void posicionarNodo(TColaCP cola, TNodo nuevo, int nivel){
    int nivelCompleto = (2^(nivel+1))-1;
    TNodo actual = cola->raiz;
    if(cola->cantidad_elementos == nivelCompleto){
        //insertar en un nuevo nivel h+1
        for(int i = 0; i < nivel; i++){actual = actual->hijo_izquierdo;}
        actual->hijo_izquierdo = nuevo;
    }else{
        //insertar en el nivel h
        int corrimientos = nivelCompleto - cola->cantidad_elementos;
        int aux = 1;
        int mitad = 2^(nivel-1);
        while(corrimientos > 1){
            if(corrimientos > mitad) actual = actual->hijo_izquierdo;
            else actual = actual->hijo_derecho;
            //estoy en un subArbol, por eso disminuyo la variable mitad y corrimientos
            mitad = mitad/2;
            corrimientos = corrimientos - (2^(nivel-aux));
            aux++;
            //si la mitad == 1 estoy en el padre del nodo a insertar, verifico en que hijo deberia insertar
            if(mitad == 1){
                if(corrimientos < mitad)//insertar hi
                    actual->hijo_izquierdo = nuevo;
                else//insertar hd
                    actual->hijo_derecho = nuevo;
            }
        }
    }
}

int cpInsertar(TColaCP cola, TEntrada entr){
    if(cola == NULL){exit(CCP_NO_INI);}

    if(cpCantidad(cola) == 0){
        //inserto en la raiz
        TNodo nuevo= (TNodo)malloc(sizeof(struct TNodo));
        cola->raiz = nuevo;
        cola->raiz->entrada = entr;
        cola->raiz->hijo_derecho = ELE_NULO;
        cola->raiz->hijo_izquierdo = ELE_NULO;
        cola->cantidad_elementos++;
        return TRUE;
    }else{
        TNodo nuevo = malloc(sizeof(struct TNodo));
        nuevo->entrada = entr;
        nuevo->hijo_derecho = ELE_NULO;
        nuevo->hijo_izquierdo = ELE_NULO;

        int nivel = (int)(log(cola->cantidad_elementos)/log(2));
        int lleno = ((log(cola->cantidad_elementos+1)/log(2))/nivel) == 1;

        //si lleno es true, debo insertar en un nuevo nivel
        if(lleno){
            TNodo actual = cola->raiz;
            for(int i = 0; i < nivel; i++) actual = actual->hijo_izquierdo; //busco el nodo mas a la izquierda del ultimo nivel
            actual->hijo_izquierdo = nuevo;
            cola->cantidad_elementos++;
        }else{
            posicionarNodo(cola, nuevo, nivel);
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
    if(cola == NULL) exit(CCP_NO_INI);
    if(cola->cantidad_elementos == 0)
        return ELE_NULO;

    TEntrada ret = cola->raiz->entrada;
    TNodo aux = cola->raiz;
    if(cola->cantidad_elementos == 1){
        cola->raiz = ELE_NULO;
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
    if(cola == NULL) exit(CCP_NO_INI);
    return cola->cantidad_elementos;
}
void cpDestruir(TColaCP cola, void (*fEliminar)(TEntrada)){
    if(cola == NULL) exit(CCP_NO_INI);
    while(cola->cantidad_elementos != 0){
        //empezar por hd de la raiz hasta q no tenga hijos
        fEliminar(cola->raiz->entrada);
        cola->raiz = cola->raiz->hijo_derecho;
    }
}


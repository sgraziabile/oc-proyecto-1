#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "colacp.h"

#define FALSE 0
#define TRUE 1
#define CCP_NO_INI 2
#define POS_NULA NULL
#define ELE_NULO NULL




TColaCP crearColaCp(int (*f)(TEntrada, TEntrada)){
    TColaCP cola = (TColaCP)malloc(sizeof(struct cola_con_prioridad));
    cola->raiz = ELE_NULO;
    cola->cantidad_elementos = 0;
    cola->comparador = f;
    return cola;
}

void posicionarNodo(TColaCP cola, TNodo* nuevo, int nivel){
    int nivelCompleto = pow(2, nivel+1) -1;
    TNodo actual = cola->raiz;
    //insertar en el nivel h
    int corrimientos = nivelCompleto - cola->cantidad_elementos;
    int mitad = pow(2,(nivel-1));
    int inserte = FALSE;

    while(!inserte){
        if(mitad != 1){
            if(corrimientos > mitad){
                actual = actual->hijo_izquierdo;
                corrimientos -= mitad;
            }
            else if(corrimientos < mitad)
                actual = actual->hijo_derecho;
            //estoy en un subArbol, por eso disminuyo la variable mitad
            mitad = mitad/2;
        }
        //si la mitad == 1 estoy en el padre del nodo a insertar, verifico en que hijo deberia insertar
        if(mitad == 1){
            if(corrimientos > mitad){//insertar hi
                actual->hijo_izquierdo = *nuevo;
                (*nuevo)->padre = actual;
            }else{//insertar hd
                actual->hijo_derecho = *nuevo;
                (*nuevo)->padre = actual;
            }
            inserte = TRUE;
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
        cola->raiz->padre = POS_NULA;
        cola->cantidad_elementos++;
        return TRUE;
    }else{
        TNodo nuevo = (TNodo)malloc(sizeof(struct TNodo));
        nuevo->entrada = entr;
        nuevo->hijo_derecho = ELE_NULO;
        nuevo->hijo_izquierdo = ELE_NULO;

        int nivel = (int)(log10(cola->cantidad_elementos)/log10(2));
        int nivelLleno = pow(2, nivel+1)-1;
        int lleno = cola->cantidad_elementos ==  nivelLleno;
        //si lleno es true, debo insertar en un nuevo nivel
        if(lleno){
            TNodo actual = cola->raiz;
            while(actual->hijo_izquierdo != ELE_NULO)  //busco el nodo mas a la izquierda del ultimo nivel
                actual = actual->hijo_izquierdo;
            actual->hijo_izquierdo = nuevo;
            nuevo->padre = actual;
            cola->cantidad_elementos++;
        }else{
            posicionarNodo(cola, &nuevo, nivel);
            cola->cantidad_elementos++;
        }

        while((nuevo->padre != NULL) && cola->comparador(nuevo->padre->entrada, nuevo->entrada) == 1){
            TEntrada aux = nuevo->padre->entrada;
            nuevo->padre->entrada = nuevo->entrada;
            nuevo->entrada = aux;
            nuevo = nuevo->padre;
        }
        return TRUE;
    }
    return FALSE;
}

TNodo buscarUltimo(TColaCP cola, int nivel){
    TNodo ult = cola->raiz;
    int nivelCompleto = pow(2,(nivel+1))-1;
    if(cola->cantidad_elementos == nivelCompleto){for(int i = 0; i < nivel; i++)ult = ult->hijo_derecho; }
    else{
        //insertar en el nivel h
        int corrimientos = nivelCompleto - cola->cantidad_elementos;
        int mitad = pow(2,nivel-1);
        int encontre = FALSE;
        while(!encontre){
            if(mitad != 1){
                if(corrimientos > mitad){
                    ult = ult->hijo_izquierdo;
                    corrimientos -= mitad;
                }
                else{
                    ult = ult->hijo_derecho;
                }
                mitad = mitad/2;
            }
            if(mitad == 1){
                if(corrimientos > mitad && ult->hijo_izquierdo != ELE_NULO)
                    ult = ult->hijo_izquierdo;
                else if (ult->hijo_derecho != ELE_NULO)
                    ult = ult->hijo_derecho;
                encontre = TRUE;
            }
        }
    }
    return ult;
}

TEntrada cpEliminar(TColaCP cola){
    if(cola == NULL) exit(CCP_NO_INI);
    if(cola->cantidad_elementos == 0)
        return ELE_NULO;

    TEntrada ret = cola->raiz->entrada;
    TNodo aux = cola->raiz;

    if(cola->cantidad_elementos == 1){
        cola->raiz = ELE_NULO;
    }else{
        int nivel = (int)(log10(cola->cantidad_elementos)/log10(2));
        TNodo ult = buscarUltimo(cola, nivel);
        ult->hijo_derecho = cola->raiz->hijo_derecho;
        ult->hijo_izquierdo = cola->raiz->hijo_izquierdo;
        ult->padre = POS_NULA;
        cola->raiz = ult;

        int huboSwap = FALSE;
        int hayHi = FALSE;
        int hayHd = FALSE;
        int mayorPrioridad = -2;

        //verifico cual de los hijos es distinto de nulo
        if(ult->hijo_izquierdo != NULL){
            hayHi = TRUE;
            ult->hijo_izquierdo->padre = ult;
        }
        if(ult->hijo_derecho != NULL){
            hayHd = TRUE;
            ult->hijo_derecho->padre = ult;
        }

        if(hayHi && hayHd){
            mayorPrioridad = cola->comparador(ult->hijo_izquierdo->entrada, ult->hijo_derecho->entrada);
        }else if(hayHi)
            mayorPrioridad = -1;
        else if(hayHd)
            mayorPrioridad = 1;

        if(mayorPrioridad != -2)
            huboSwap = TRUE;

        //burbujeo hacia abajo, reordeno el heap
        if(huboSwap){
            huboSwap = FALSE;
            if(mayorPrioridad == 1 && cola->comparador(ult->entrada, ult->hijo_derecho->entrada)){
                TEntrada aux = ult->hijo_derecho->entrada;
                ult->hijo_derecho->entrada = ult->entrada;
                ult->entrada = aux;
                ult = ult->hijo_derecho;
                huboSwap = TRUE;
            }else if(mayorPrioridad == -1 && cola->comparador(ult->entrada, ult->hijo_izquierdo->entrada)){
                TEntrada aux = ult->hijo_izquierdo->entrada;
                ult->hijo_izquierdo->entrada = ult->entrada;
                ult->entrada = aux;
                ult = ult->hijo_izquierdo;
                huboSwap = TRUE;
            }
        }
    }
    cola->cantidad_elementos--;
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
        int nivel = (int)(log(cola->cantidad_elementos)/log(2));
        TNodo aux = buscarUltimo(cola, nivel);
        fEliminar(aux->entrada);
        free(aux);
    }
    free(cola);
}




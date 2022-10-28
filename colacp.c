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

    if(cola->cantidad_elementos == nivelCompleto){
        //insertar en un nuevo nivel h+1
        for(int i = 0; i < nivel; i++){actual = actual->hijo_izquierdo;}
        actual->hijo_izquierdo = *nuevo;
        (*nuevo)->padre = actual;
    }else{
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

        int nivel = (int)(log10(cola->cantidad_elementos)/log10(2));
        int lleno = ((log10(cola->cantidad_elementos+1)/log10(2))/nivel) == 1;

        //si lleno es true, debo insertar en un nuevo nivel
        if(lleno){
            TNodo actual = cola->raiz;
            for(int i = 0; i < nivel; i++) actual = actual->hijo_izquierdo; //busco el nodo mas a la izquierda del ultimo nivel
            actual->hijo_izquierdo = nuevo;
            nuevo->padre = actual;
            cola->cantidad_elementos++;

        }else{
            posicionarNodo(cola, &nuevo, nivel);
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

TNodo buscarUltimo(TColaCP cola, int nivel){
    TNodo ult = cola->raiz;
    int nivelCompleto = pow(2,(nivel+1))-1;
    if(cola->cantidad_elementos == nivelCompleto){for(int i = 0; i < nivel; i++)ult = ult->hijo_derecho; }
    else{
        //insertar en el nivel h
        int corrimientos = nivelCompleto - cola->cantidad_elementos +1;
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
                if(corrimientos > mitad)
                    ult = ult->hijo_izquierdo;
                else
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
        cola->raiz = ult;
        int huboSwap = FALSE;
        if(ult->hijo_izquierdo != NULL || ult->hijo_derecho != NULL) //si alguno de los dos hijos es distinto de nulo
            huboSwap = TRUE;

        //burbujeo hacia abajo, reordeno el heap
        while(huboSwap){
            huboSwap = FALSE;
            int prioridadMin;
            if(ult->hijo_izquierdo != ELE_NULO && ult->hijo_derecho != ELE_NULO)
                prioridadMin = cola->comparador(ult->hijo_izquierdo->entrada, ult->hijo_derecho->entrada);
            else if (ult->hijo_izquierdo != ELE_NULO)
                prioridadMin = 1;
            else
                prioridadMin = -1;
            if(prioridadMin == 1){ //el hi tiene mayor prioridad
                if(cola->comparador(ult->entrada, ult->hijo_izquierdo->entrada) == 1){
                    TNodo hd = ult->hijo_derecho;
                    TNodo hi = ult->hijo_izquierdo;
                    ult->hijo_derecho = hi->hijo_derecho;
                    ult->hijo_izquierdo = hi->hijo_izquierdo;
                    hi->hijo_izquierdo = ult;
                    hi->hijo_derecho = hd;
                    hd->padre = hi;
                    if(ult->padre != POS_NULA){
                        //hago swap entre nodos internos
                        TNodo padreAnterior = ult->padre;
                        padreAnterior->hijo_izquierdo = hi;
                        hi ->padre = padreAnterior;
                    }else{
                        //hago swap con la raiz
                        hi->padre = POS_NULA;
                        cola->raiz = hi;
                    }
                    ult->padre = hi;
                    huboSwap = TRUE;
                }
            }else{ //el hd tiene mayor prioridad o las prioridades son iguales
                if(cola->comparador(ult->entrada, ult->hijo_derecho->entrada) == 1){
                    TNodo hd = ult->hijo_derecho;
                    TNodo hi = ult->hijo_izquierdo;
                    ult->hijo_derecho = hd->hijo_derecho;
                    ult->hijo_izquierdo = hd->hijo_izquierdo;
                    //SON VARIABLES LOCALES X ESO SE BORRAN
                    //MUESTRA SALIQUELO Y DESP LA RAIZ QUEDA CON HIJOS NULOS
                    //PQ LE ASIGNO ULT->HIJO_DERECHO = HD... PERO ESE HD SE BORRA
                    hd->hijo_derecho = ult;
                    hd->hijo_izquierdo = hi;
                    hi->padre = hd;
                    if(ult->padre != POS_NULA){
                        TNodo padreAnterior = ult->padre;
                        padreAnterior->hijo_derecho = hd;
                        hd->padre = padreAnterior;
                    }else{
                        hd->padre = POS_NULA;
                        cola->raiz = hd;
                    }
                    ult->padre = hd;
                    if(ult->hijo_izquierdo != NULL || ult->hijo_derecho != NULL) //si alguno de los dos hijos es distinto de nulo
                        huboSwap = TRUE;

                }
            }
        }
    }
    cola->cantidad_elementos--;
    //free(aux);
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




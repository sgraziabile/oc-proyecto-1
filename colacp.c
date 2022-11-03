#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "colacp.h"

TColaCP crear_cola_cp(int (*f)(TEntrada, TEntrada)){
    TColaCP cola = (TColaCP)malloc(sizeof(struct cola_con_prioridad));
    cola->raiz = ELE_NULO;
    cola->cantidad_elementos = 0;
    cola->comparador = f;
    return cola;
}

void posicionarNodo(TColaCP cola, TNodo nuevo, int nivel){
    int nivelCompleto = pow(2, nivel+1)-1; //la cantidad de nodos posibles si el nivel está completo
    TNodo actual = cola->raiz;
    //insertar en el nivel h
    int corrimientos = nivelCompleto - cola->cantidad_elementos; //representa la cantidad de nodos que tengo que moverme de derecha a izquierda hasta llegar al buscado
    int mitad = pow(2,(nivel-1)); //representa la mitad de la cantidad de nodos posibles en el último nivel
    int encontre = FALSE;
    int inserte = FALSE;

    while(!inserte){
        if(mitad != 1){
            if(corrimientos > mitad){
                actual = actual->hijo_izquierdo;
                corrimientos -= mitad;
            }
            else
                actual = actual->hijo_derecho;
            //estoy en un subArbol, por eso disminuyo la variable mitad
            mitad = mitad/2;
        }
        //si la mitad == 1 estoy en el padre del nodo a insertar, verifico en que hijo deberia insertar
        if(mitad == 1){
            if(corrimientos > mitad){//insertar hi
                actual->hijo_izquierdo = nuevo;
                nuevo->padre = actual;
            }else{//insertar hd
                actual->hijo_derecho = nuevo;
                nuevo->padre = actual;
            }
            inserte = TRUE;
        }
    }
}

int cp_insertar(TColaCP cola, TEntrada entr){
    if(cola == NULL)
        exit(CCP_NO_INI);

    if(cp_cantidad(cola) == 0){
        //inserto en la raiz
        TNodo nuevo= (TNodo)malloc(sizeof(struct nodo));
        cola->raiz = nuevo;
        cola->raiz->entrada = entr;
        cola->raiz->hijo_derecho = ELE_NULO;
        cola->raiz->hijo_izquierdo = ELE_NULO;
        cola->raiz->padre = POS_NULA;
        cola->cantidad_elementos++;
        return TRUE;
    }else{
        TNodo nuevo = (TNodo)malloc(sizeof(struct nodo));
        nuevo->entrada = entr; //ya se reservó memoria
        nuevo->hijo_derecho = ELE_NULO;
        nuevo->hijo_izquierdo = ELE_NULO;

        int nivel = (int)(log10(cola->cantidad_elementos)/log10(2)); //fórmula para calcular el nivel del árbol
        int nivelLleno = pow(2, nivel+1)-1; //cantidad de nodos que tiene el árbol si el último nivel está lleno
        int lleno = cola->cantidad_elementos ==  nivelLleno;
        //si lleno es true, debo insertar en un nuevo nivel
        if(lleno){
            TNodo actual = cola->raiz;
            while(actual->hijo_izquierdo != ELE_NULO)  //busco el nodo mas a la izquierda del ultimo nivel
                actual = actual->hijo_izquierdo;
            actual->hijo_izquierdo = nuevo;
            nuevo->padre = actual;
        }else
            posicionarNodo(cola, nuevo, nivel);
        cola->cantidad_elementos++;

        while(nuevo->padre != NULL && cola->comparador(nuevo->padre->entrada, nuevo->entrada) == 1){
            //swapeo hacia arriba
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
    int nivelCompleto = pow(2,(nivel+1))-1; //la cantidad de nodos posibles si el nivel está completo
    if(cola->cantidad_elementos == nivelCompleto){     //si esta completo, el último está en el extremo derecho del árbol
        while(ult->hijo_derecho != ELE_NULO)
            ult = ult->hijo_derecho;
    }
    else{
        int corrimientos = nivelCompleto - cola->cantidad_elementos +1; //representa la cantidad de nodos que tengo que moverme de derecha a izquierda hasta llegar al buscado
        int mitad = pow(2,nivel-1); //representa la mitad de la cantidad de nodos posibles en el último nivel
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
                mitad = mitad/2; //me moví a un sub-árbol, por lo tanto reduzco la mitad
            }
           if(mitad == 1){ //estoy en el padre del nodo buscado
                if(corrimientos > mitad){
                    ult = ult->hijo_izquierdo;
                }else
                    ult = ult->hijo_derecho;
                encontre = TRUE;
            }
        }
    }
    return ult;
}

TEntrada cp_eliminar(TColaCP cola){
    if(cola == NULL) exit(CCP_NO_INI);
    if(cola->cantidad_elementos == 0)
        return ELE_NULO;

    TEntrada ret = cola->raiz->entrada;
    TNodo aux = NULL;

    if(cola->cantidad_elementos == 1){
        cola->raiz = ELE_NULO;
    }else{
        int nivel = (int)(log10(cola->cantidad_elementos)/log10(2)); //formula para calcular el nivel del árbol
        aux = buscarUltimo(cola, nivel);
        if(aux->padre->hijo_izquierdo == aux)
            aux->padre->hijo_izquierdo = ELE_NULO;
        else
            aux->padre->hijo_derecho = ELE_NULO;
        cola->raiz->entrada = aux->entrada;
        TNodo actual = cola->raiz;

        int huboSwap = FALSE;
        int hayHi = FALSE;
        int hayHd = FALSE;
        int mayorPrioridad = -2;
        //verifico cual de los hijos es distinto de nulo
        if(actual->hijo_izquierdo != NULL){
            hayHi = TRUE;
        }
        if(actual->hijo_derecho != NULL){
            hayHd = TRUE;
        }

        if(hayHi && hayHd){
            mayorPrioridad = cola->comparador(actual->hijo_izquierdo->entrada, actual->hijo_derecho->entrada);
        }else if(hayHi)
            mayorPrioridad = -1;
        else if(hayHd)
            mayorPrioridad = 1;

        if(mayorPrioridad != -2)
            huboSwap = TRUE;
        if(mayorPrioridad == 0){
            mayorPrioridad = 1;
        }
        //burbujeo hacia abajo, reordeno el heap
        if(huboSwap){
            huboSwap = FALSE;
            if(mayorPrioridad == 1 && cola->comparador(actual->entrada, actual->hijo_derecho->entrada) == 1){
                TEntrada auxiliar = actual->hijo_derecho->entrada;
                actual->hijo_derecho->entrada = actual->entrada;
                actual->entrada = auxiliar;
                actual = actual->hijo_derecho;
                huboSwap = TRUE;
            }else if(mayorPrioridad == -1 && cola->comparador(actual->entrada, actual->hijo_izquierdo->entrada) == 1){
                TEntrada auxiliar = actual->hijo_izquierdo->entrada;
                actual->hijo_izquierdo->entrada = actual->entrada;
                actual->entrada = auxiliar;
                actual = actual->hijo_izquierdo;
                huboSwap = TRUE;
            }
        }
    }
    cola->cantidad_elementos--;
    free(aux);
    return ret;
}

int cp_cantidad(TColaCP cola){
    if(cola == NULL)
        exit(CCP_NO_INI);
    return cola->cantidad_elementos;
}

void destruirRec(TNodo actual, void (*fEliminar)(TEntrada)){
    if(actual->hijo_izquierdo != NULL)
        destruirRec(actual->hijo_izquierdo, fEliminar);
    if(actual->hijo_derecho != NULL)
        destruirRec(actual->hijo_derecho, fEliminar);
    fEliminar(actual->entrada);
    free(actual);
}
void cp_destruir(TColaCP cola, void (*fEliminar)(TEntrada)){
    if(cola == NULL)
        exit(CCP_NO_INI);
    destruirRec(cola->raiz, fEliminar);
    free(cola);
}




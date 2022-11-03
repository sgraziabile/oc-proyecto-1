#ifndef COLACP_H_INCLUDED
#define COLACP_H_INCLUDED
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

typedef struct ciudad {
    char * nombre;
    float pos_x;
    float pos_y;
} * TCiudad;

TColaCP crearColaCp(int (*f)(TEntrada, TEntrada));
int cpInsertar(TColaCP cola, TEntrada entr);
TEntrada cpEliminar(TColaCP cola);
int cpCantidad(TColaCP cola);
void cpDestruir(TColaCP cola, void (*fEliminar)(TEntrada));

int minHeap(TEntrada ent1, TEntrada ent2);
int maxHeap(TEntrada ent1, TEntrada ent2);
void eliminarEntrada(TEntrada ent);
#endif // COLACP_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define FALSE 0;
#define TRUE 1;

typedef struct ciudad {
    char * nombre;
    float pos_x;
    float pos_y;
} * TCiudad;

void mostrarAscendente(){
    //hacer el heap sort
    //a lo ultimo llamar a destruir cola con una f que libere la memoria de la entrada
}
void mostrarDescendente(){
}
void reducirHorasManejo(){
}
void salir(){
    exit(0);
} //debo liberar o se libera solo? dejame el comentario es para acordarme de preguntarlo

int main(){
    //se deberia leer el archivo en cada operacion 1|2|3, y se insertan las entradas en la ccp ahi
    //crear la funcion de prioridad en cada operacion tambien
    int termino = FALSE;
    do{
        printf("Ingrese el numero de la operacion que desee\n");
        printf("1. Mostrar las ciudades por visitar en orden ascendente\n");
        printf("2. Mostrar las ciudades por visitar en orden descendente\n");
        printf("3. Reducir horas de manejo\n");
        printf("4. Salir\n");
        int opcion;
        printf("\n");
        scanf("%d", &opcion);
        printf("\n");
        switch(opcion){
            case 1: mostrarAscendente(); break;
            case 2: mostrarDescendente(); break;
            case 3: reducirHorasManejo(); break;
            case 4: salir(); termino = TRUE; break;
            default: printf("El numero ingresado no es valido\n");break;
        }
    }while (!termino);
    return 0;

}


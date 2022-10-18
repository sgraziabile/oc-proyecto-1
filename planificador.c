#include <stdio.h>
#include <stdlib.h>

#define FALSE 0;
#define TRUE 1;

typedef struct ciudad {
    char * nombre;
    float pos_x;
    float pos_y;
} * TCiudad;

void mostrarAscendente();
void mostrarDescendente();
void reducirHorasManejo();
void salir(); //debo liberar o se libera solo?

main(){
    int termino; termino = FALSE;
    do{
        printf("Ingrese el numero de la operacion que desee\n");
        printf("1. Mostrar las ciudades por visitar en orden ascendente\n");
        printf("2. Mostrar las ciudades por visitar en orden descendente\n");
        printf("3. Reducir horas de manejo");
        printf("4. Salir");
        int opcion;
        scanf("%d", &opcion);
        switch(opcion){
            case 1: mostrarAscendente(); break;
            case 2: mostrarDescendente(); break;
            case 3: reducirHorasManejo(); break;
            case 4: salir(); termino = TRUE; break;
            default: printf("El numero ingresado no es valido");
        }
    }while (termino == FALSE);
    return 0;
}


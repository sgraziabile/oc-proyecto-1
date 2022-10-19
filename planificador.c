#include <stdio.h>
#include <stdlib.h>

#define FALSE 0;
#define TRUE 1;

typedef struct ciudad {
    char * nombre;
    float pos_x;
    float pos_y;
} * TCiudad;

void crearCiudad(int x, int y, char * nombre) {
        TCiudad ciudad = (TCiudad)malloc(sizeof(struct ciudad));
        ciudad->pos_x = x;
        ciudad->pos_y = y;

}

void mostrarAscendente(){
    //hacer el heap sort
    //a lo ultimo llamar a destruir cola con una f que libere la memoria de la entrada
    FILE * ptr;
    ptr = fopen("archivo_texto.txt","r");
    if(ptr != NULL) {
        char linea[60];
        char pos_x[5]; char pos_y[5]; //para guardar las posiciones leidas y convertirlas a float
        char nombre[50];    //guardar memoria?

        int sizeLinea = sizeof(linea) / sizeof(char);
        int sizeNombre = sizeof(nombre) / sizeof(char);
        int sizeNumeros = sizeof(pos_x) / sizeof(char);

        //posiciones
        float actual_x;
        float actual_y;
        int x;          //las declaro cuando leo cada linea;
        int y;

        int i = 0; int j = 0; //j recorre los arreglos de las posiciones
        int fin = 0;
        fgets(linea, 60, ptr);      //primera linea con la posicion
        while(i < 60 && j < 5 && fin == 0) {
            pos_x[j] = linea[i];
            i++; j++;
            if(linea[i] == ';') {
                actual_x = atof(pos_x);
                fin = 1;
            }
        }
        i++;
        j = 0;
        while(i < 60 && j < 5) {
            pos_y[j] = linea[i];
            i++; j++;
            }
        actual_y = atof(pos_y);

        fin = 0; j = 0; fin = 0;
        while(!feof(ptr)) {     //leer cada linea, guardar las variables y llamar a crearCiudad()
            char x[5]; char y[5];
            fgets(linea, 60, ptr);
            while(i < 60 && j < 5 && fin == 0) {
                nombre[j] = linea[i];
                i++; j++;
                if(linea[i] == ';') { //
                    //char * ptrNombre = (*char)malloc(sizeof(sizeNombre +1));
                }
            }

        }









    }
        fclose(ptr);

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
    //hacer malloc para la cadena de char del nombre de la ciudad
    //ver el tema de $>planificador <archvo_texto>


    int termino = FALSE;
    do{
        printf("Ingrese el numero de la operacion que desee realizar: \n");
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


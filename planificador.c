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
void vaciar(char temp[]) {          //vacia un arreglo de char
    int i;
    for(i = 0; i < 50; i++)
        temp[i] = '\0';
}
void copiar(char temp[]; int i) {
    int N = strlen(temp) + 1;       guarda la longitud de temp + '\0'
    ciudad
}

void mostrarAscendente(){
    //hacer el heap sort
    //a lo ultimo llamar a destruir cola con una f que libere la memoria de la entrada
    int i; int j;
    int cont = -1;
    char temp[50];
    char aux;
    FILE * ptr;
    ptr = fopen("archivo_texto.txt","r");
    if(ptr != NULL) {
        while(!feof(ptr)) {             //cuento la cantidad de ciudades
            fgets(temp, 50, ptr);
            cont++;
        }
        rewind(ptr);                    //vuelvo al principio del archivo
        TCiudad ciudad;
        ciudad = (TCiudad)malloc(cont * sizeof(struct ciudad));
        if(ciudad == NULL)
            printf("No se pudo reservar memoria. \n");
        else {
            vaciar(temp);
            fgets(temp, 50, ptr);       //me salto la primera linea
            for(i = 0; !feof(f); i++) {
                vaciar(temp);
                aux = '0';
                for(j = 0; aux != ';'; j++) {
                    aux = fget(ptr);
                    if(aux != ';') {
                        temp[j] = aux;          //guardo cada char del nombre en temp;
                    }
                }
                copiar(temp, i);
            }

        }





        fclose(ptr);
    }
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


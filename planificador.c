#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colacp.h"
#include "math.h"

#define FALSE 0
#define TRUE 1

int minHeap(TEntrada ent1, TEntrada ent2){
    if(ent2->valor > ent1->valor)
        return -1;
    else if(ent1->valor > ent2->valor)
        return 1;
    else return 0;
}

void crearCiudad(float x, float y, char * nombre){
        TCiudad ciudad = (TCiudad)malloc(sizeof(struct ciudad));
        ciudad->pos_x = x;
        ciudad->pos_y = y;
}

TEntrada crearEntrada(TClave clave,TValor valor) {
    TEntrada entrada = (TEntrada)malloc(sizeof(struct entrada));
    entrada->clave = clave;
    entrada->valor = valor;
    return entrada;
}

void vaciar(char temp[]) {          //vacia un arreglo de char
    int i;
    for(i = 0; i < 50; i++)
        temp[i] = '\0';
}
TCiudad guardarCiudades() {
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
        ciudad = (TCiudad)malloc(cont * sizeof(struct ciudad)); //si no los meto en un arreglo aca reservo para uno solo.
        if(ciudad == NULL)
            printf("No se pudo reservar memoria. \n");
        else {
            vaciar(temp);
            fgets(temp, 50, ptr);           //me salto la primera linea
            for(i = 0; !feof(ptr); i++) {   //los inserto en un arreglo, si los meto directo a la ccp esto no va
                vaciar(temp);
                aux = '0';
                for(j = 0; aux != ';'; j++) {
                    aux = fgetc(ptr);
                    if(aux != ';') {
                        temp[j] = aux;          //guardo cada char del nombre en temp;
                    }
                }
                //guardo el nombre
                int N = strlen(temp) + 1;       //long de temp + '\0'
                ciudad[i].nombre = (char*)malloc(N * sizeof(char));
                if(ciudad[i].nombre != NULL)
                    strcpy(ciudad[i].nombre, temp);
                //guardo x
                vaciar(temp);
                aux = '0';
                for(j = 0; aux != ';'; j++) {
                    aux = fgetc(ptr);
                    if(aux != ';')
                        temp[j] = aux;
                }
                ciudad[i].pos_x = atof(temp);
                //guardo y
                vaciar(temp);
                fgets(temp,7,ptr);           //lee el resto de la linea
                ciudad[i].pos_y = atof(temp);

                printf("Ciudad: %s (%.2f,%.2f) \n",ciudad[i].nombre,ciudad[i].pos_x,ciudad[i].pos_y);
            }
        }
        return ciudad;
    }
    fclose(ptr);
    return NULL;//te cambie el return pq no andaba por tus llaves
}
TEntrada guardarPosicion() {
    int j;
    char temp[50];
    char aux;
    FILE * ptr;
    ptr = fopen("archivo_texto.txt","r");  //argv[1]
    //leo el x de la posicion
    aux = '0';
    for(j = 0; aux != ';'; j++) {
    aux = fgetc(ptr);
        if(aux != ';')
            temp[j] = aux;
    }
    float x_actual = atof(temp);
    //leo el y de la posicion
    vaciar(temp);
    fgets(temp,7,ptr);
    float y_actual = atof(temp);
    printf("Posicion: %.2f - %.2f \n", x_actual, y_actual);
    fclose(ptr);

    TEntrada entrada = (TEntrada)malloc(sizeof(struct entrada));
    entrada->clave = (TClave*)malloc(sizeof(x_actual));
    entrada->valor = (TValor*)malloc(sizeof(y_actual));
    return entrada;
}
/*float calcularDistancia(TClave pos_x, TValor pos_y, TCiudad ciudad) {
    float distancia =(float)(ciudad->pos_x) - (float)*pos_x +(float)(ciudad->pos_y)* - pos_y;
    return distancia;
}*/
void mostrarAscendente(){
    //hacer el heap sort
    //a lo ultimo llamar a destruir cola con una f que libere la memoria de la entrada
    int i; float distancia;
    TColaCP cola = crearColaCp(minHeap);
    TCiudad ciudad = guardarCiudades();
    int cantCiudades = sizeof(struct ciudad) / sizeof(ciudad[0]);
    TEntrada entrada = guardarPosicion();
    for(i = 0; i < cantCiudades; i++) {
       // distancia = calcularDistancia(entrada->clave, entrada->valor, ciudad[i]);
       distancia = i;
        TEntrada entrada = crearEntrada((TClave)&ciudad[i], (TValor)&distancia);
        cpInsertar(cola, entrada);
    }

    //termina la lectura
}

void mostrarDescendente(){
}
void reducirHorasManejo(){
}
void salir(){
    exit(0);
} //debo liberar o se libera solo? dejame el comentario es para acordarme de preguntarlo

int main(int argc, char *argv[]){
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "colacp.h"
#include "math.h"

#define FALSE 0
#define TRUE 1

int minHeap(TEntrada ent1, TEntrada ent2){
    if(*(float*)ent2->valor > *(float*)ent1->valor)
        return -1;
    else if(*(float*)ent1->valor > *(float*)ent2->valor)
        return 1;
    else return 0;
}

int maxHeap(TEntrada ent1, TEntrada ent2){
    if(*(float*)ent2->valor > *(float*)ent1->valor)
        return 1;
    else if(*(float*)ent1->valor > *(float*)ent2->valor)
        return -1;
    else return 0;
}

void eliminarEntrada(TEntrada ent){
    free(ent->valor);
    free(ent);
}

TEntrada crearEntrada(TClave clave, TValor valor) {
    TEntrada entrada = (TEntrada)malloc(sizeof(struct entrada));
    entrada->valor = valor;         //el valor y la clave ya tienen su correspondiente malloc
    entrada->clave = clave;
    return entrada;
}

void vaciar(char temp[]) {          //vacia un arreglo de char
    int i;
    for(i = 0; i < 100; i++)
        temp[i] = '\0';
}

TCiudad guardarCiudades(int *size, char * archivo) {
    int i; int j;
    int cont = 0;
    char temp[100];
    char aux;
    TCiudad ciudad = NULL;
    FILE * ptr;
    ptr = fopen(archivo, "r");
    if(ptr != NULL) {
        while(!feof(ptr)) {             //cuento la cantidad de ciudades
            fgets(temp, 100, ptr);
            cont++;
        }
        *size = cont ;                  //guardo la longitud del arreglo
        rewind(ptr);                    //vuelvo al principio del archivo
        ciudad = (TCiudad)malloc(cont * sizeof(struct ciudad)); //
        if(ciudad == NULL)
            printf("No se pudo reservar memoria. \n");
        else {
            vaciar(temp);
            fgets(temp, 100, ptr);           //me salto la primera linea ya que es el origen
            for(i = 1; !feof(ptr); i++) {
                vaciar(temp);
                aux = '0';
                for(j = 0; aux != ';'; j++) {
                    aux = fgetc(ptr);
                    if(aux != ';') {
                        temp[j] = aux;       //guardo cada char del nombre en temp;
                    }
                }
                //guardo el nombre
                int N = strlen(temp)+1;       //long de temp + '\0'
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
                fgets(temp,10,ptr);           //lee el resto de la linea
                ciudad[i].pos_y = atof(temp);
            }
                //Leo y guardo la posicion
                rewind(ptr);                //vuelvo al principio del archivo
                vaciar(temp);
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
                ciudad[0].nombre = "Posicion";
                ciudad[0].pos_x = x_actual;
                ciudad[0].pos_y = y_actual;
        }
    }
    fclose(ptr);
    return ciudad;
}

float calcularDistancia(float pos_x, float pos_y, float ciudad_x, float ciudad_y) {
    float distancia = abs(ciudad_x - pos_x) + abs(ciudad_y - pos_y);
    return distancia;
}

void mostrarAscendente(TCiudad ciudades, int * cantCiudades){
    TColaCP cola = crearColaCp(minHeap);
    for(int i = 1; i < *cantCiudades; i++) {
        float* distancia = (float*) malloc(sizeof(float));
        *distancia = calcularDistancia(ciudades[0].pos_x,ciudades[0].pos_y, ciudades[i].pos_x, ciudades[i].pos_y);
        TEntrada entrada = crearEntrada((TClave)&ciudades[i], (TValor)distancia);    //Entrada (TCiudad, Distancia)
        cpInsertar(cola, entrada);
    }
    printf("Mostrar ascendente: \n");
    for(int i = 1; cola->cantidad_elementos > 0; i++) {
        TEntrada ent = cpEliminar(cola);
        printf("%d. %s \n",i,((TCiudad)ent->clave)->nombre);
        eliminarEntrada(ent);
    }
   free(cola);
}

void mostrarDescendente(TCiudad ciudades, int * cantCiudades){
    TColaCP cola = crearColaCp(maxHeap);
    for(int i = 1; i < *cantCiudades; i++) {
        float* distancia = (float*) malloc(sizeof(float));
        *distancia = calcularDistancia(ciudades[0].pos_x,ciudades[0].pos_y, ciudades[i].pos_x, ciudades[i].pos_y);
        TEntrada entrada = crearEntrada((TClave)&ciudades[i], (TValor)distancia);     //Entrada (TCiudad, Distancia)
        cpInsertar(cola, entrada);
    }
    printf("Mostrar descendente: \n");
    for(int i = 1; cola->cantidad_elementos > 0; i++) {
        TEntrada ent = cpEliminar(cola);
        printf("%d. %s \n",i,((TCiudad)ent->clave)->nombre);
        eliminarEntrada(ent);
    }
    free(cola);
}

void insertarEnCola(TColaCP cola, int * cantCiudades, TCiudad* visitados, float xOrigen, float yOrigen){
    //funcion utilizada para reducir horas de manejo
    for(int i = 1; i < *cantCiudades; i++) {
        if(visitados[i] != POS_NULA){
            TCiudad ciudad = visitados[i];
            float* distancia = (float*) malloc(sizeof(float));
            *distancia = calcularDistancia(xOrigen, yOrigen, ciudad->pos_x, ciudad->pos_y);
            TEntrada entrada = crearEntrada((TClave)ciudad, (TValor)distancia);   //Entrada (TCiudad, Distancia)
            cpInsertar(cola, entrada);
        }
    }
}

void reducirHorasManejo(TCiudad ciudades, int * cantCiudades){
    float xOrigen = ciudades->pos_x;
    float yOrigen = ciudades->pos_y;
    float distanciaTotal = 0;

    TCiudad* visitados = (TCiudad*)malloc((*cantCiudades) * sizeof(TCiudad));
    for(int i = 1; i < *cantCiudades; i++){ //creamos un arreglo que apunte a las ciudades ya guardadas en la caché
        visitados[i] = (ciudades+i);
    }
    printf("Reducir horas de manejo: \n");
    for(int i = 1; i < *cantCiudades; i++){
        TColaCP cola = crearColaCp(minHeap);
        insertarEnCola(cola, cantCiudades, visitados, xOrigen, yOrigen);
        TEntrada ent = cpEliminar(cola);
        printf("%d. %s \n",i,((TCiudad)ent->clave)->nombre);
        xOrigen = ((TCiudad)ent->clave)->pos_x;
        yOrigen = ((TCiudad)ent->clave)->pos_y;
        distanciaTotal += *(float*)ent->valor;
        eliminarEntrada(ent);
        if(i != *cantCiudades-1) //cuando queda un unico elemento no llamamos a destruir porque ya fue liberado
            cpDestruir(cola, eliminarEntrada);

        int encontre = FALSE;
        for(int j = 0; j < *cantCiudades && !encontre; j++){
            if(visitados[j] == ((TCiudad)ent->clave)){ //setteamos en posicion nula el puntero a la ciudad que ya fue visitada
                encontre = TRUE;
                visitados[j] = POS_NULA;
            }
        }
    }
    free(visitados);
    printf("Distancia recorrida: %f \n", distanciaTotal);
}

void salir(TCiudad ciudades, int cantCiudades){
    for(int i = 1; i < cantCiudades; i++){ //libero la caché
        free(ciudades[i].nombre);
    }
    free(ciudades);
    printf("Saliendo...");
    exit(0);
}

int main(int argc, char *argv[]){
    TCiudad ciudades = NULL;
    int cantCiudades = 0;
    if(argc != 2){
        printf("Hubo un error con los archivos ingresados como parametros\n");
        salir(ciudades, cantCiudades);
    }
    ciudades = guardarCiudades(&cantCiudades, argv[1]); //creamos un arreglo de ciudades que funciona como caché
    int termino = FALSE;
    do{
        printf("Ingrese el numero de la operacion que desee realizar: \n");
        printf("1. Mostrar las ciudades por visitar en orden ascendente\n");
        printf("2. Mostrar las ciudades por visitar en orden descendente\n");
        printf("3. Reducir horas de manejo\n");
        printf("4. Salir\n");
        int opcion = 0;
        printf("\n");
        scanf("%d", &opcion);
        printf("\n");
        switch(opcion){
            case 1: mostrarAscendente(ciudades, &cantCiudades); break;
            case 2: mostrarDescendente(ciudades, &cantCiudades); break;
            case 3: reducirHorasManejo(ciudades, &cantCiudades); break;
            case 4: salir(ciudades, cantCiudades); termino = TRUE; break;
            default: printf("El numero ingresado no es valido\n"); fflush(stdin);break; //limpio el buffer
        }
        printf("\n");
    }while (!termino);
    return 0;
}


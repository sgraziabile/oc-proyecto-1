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
    TCiudad ret = NULL;
    FILE * ptr;
    ptr = fopen(archivo, "r");
    //ptr = fopen("archivo_texto.txt","r");
    if(ptr != NULL) {
        while(!feof(ptr)) {             //cuento la cantidad de ciudades
            fgets(temp, 100, ptr);
            cont++;
        }
        *size = cont ;                  //guardo la longitud del arreglo
        rewind(ptr);                    //vuelvo al principio del archivo
        TCiudad ciudad = (TCiudad)malloc(cont * sizeof(struct ciudad)); //
        if(ciudad == NULL)
            printf("No se pudo reservar memoria. \n");
        else {
            vaciar(temp);
            fgets(temp, 100, ptr);           //me salto la primera linea
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
        ret = ciudad;
    }
    fclose(ptr);
    return ret;
}

float calcularDistancia(float pos_x, float pos_y, float ciudad_x, float ciudad_y) {
    float distancia = abs(ciudad_x - pos_x) + abs(ciudad_y - pos_y);
    return distancia;
}

void mostrarAscendente(TCiudad ciudades, int * cant){
    int cantCiudades = *cant; int i;
    TColaCP cola = crearColaCp(minHeap);
    for(i = 1; i < cantCiudades; i++) {
        float* distancia = (float*) malloc(sizeof(float));
        *distancia = calcularDistancia(ciudades[0].pos_x,ciudades[0].pos_y, ciudades[i].pos_x, ciudades[i].pos_y);
        TEntrada entrada = crearEntrada((TClave)&ciudades[i], (TValor)distancia);         //Entrada (TCiudad, Distancia)
        cpInsertar(cola, entrada);
    }
    printf("Mostrar ascendente: \n");
    i = 1;
    while(cola->cantidad_elementos > 0) {
        TEntrada ent = cpEliminar(cola);
        printf("%d. %s \n",i,((TCiudad)ent->clave)->nombre);
        i++;
        free(ent->valor);
    }
    //free(cola);
}

void mostrarDescendente(TCiudad ciudades, int * cant){
    int cantCiudades = *cant; int i;
    TColaCP cola = crearColaCp(maxHeap);
    for(i = 1; i < cantCiudades; i++) {
        float* distancia = (float*) malloc(sizeof(float));
        *distancia = calcularDistancia(ciudades[0].pos_x,ciudades[0].pos_y, ciudades[i].pos_x, ciudades[i].pos_y);
        TEntrada entrada = crearEntrada((TClave)&ciudades[i], (TValor)distancia);         //Entrada (TCiudad, Distancia)
        cpInsertar(cola, entrada);
    }
    printf("Mostrar descendente: \n");
    i = 1;
    while(cola->cantidad_elementos > 0) {
        TEntrada ent = cpEliminar(cola);
        printf("%d. %s \n",i,((TCiudad)ent->clave)->nombre);
        i++;
        free(ent->valor);
    }
   // cpDestruir(cola,cpEliminar); no anda
    //free(cola);
}

void reducirHorasManejo(TCiudad ciudades, int * cant){
    int cantCiudades = *cant;
    float distanciaTotal =0;
    int j = 0; //con 1 el mismo error

    TCiudad* ciudadesSV = (TCiudad*)malloc(cantCiudades * sizeof(TCiudad));      //ciudades sin visitar
    for(int i = 0; i < cantCiudades; i++){
        *(ciudadesSV+i) = (ciudades+i);
    }
    TCiudad* ciudadesV = (TCiudad*)malloc(cantCiudades * sizeof(TCiudad));      //ciudades visitadas
    float restantes = cantCiudades;                                         //Controlo la cantidad de ciudades sin visitar

    while(restantes > 1) {
        TColaCP cola = crearColaCp(minHeap);
        for(int i = 1; i < cantCiudades; i++) {
            float distancia = *(float*) malloc(sizeof(float));
            distancia = calcularDistancia(ciudadesSV[0]->pos_x,ciudadesSV[0]->pos_y, ciudadesSV[i]->pos_x, ciudadesSV[i]->pos_y);
            TEntrada entrada = crearEntrada((TClave)&ciudadesSV[i], (TValor)&distancia);         //Entrada (TCiudad, Distancia)
            cpInsertar(cola, entrada);
        }
        TEntrada ent = cpEliminar(cola);
        //actualizo la posicion
        (*ciudadesSV)->pos_x = ((TCiudad)ent->clave)->pos_x;
        (*ciudadesSV)->pos_y = ((TCiudad)ent->clave)->pos_y;

        *(ciudadesV+j) = ((TCiudad)ent->clave);
        j++;
        distanciaTotal += *(float*)ent->valor;          //aumento la distancia recorrida
        //elimino la ciudad de la lista de no visitadas
        int elimine =FALSE;
        TCiudad aux = (TCiudad)(ent->clave);
        for(int i = 1; i < restantes && !elimine; i++){
            if((ciudadesSV+i) == aux) {
                printf("Entro al if \n");
                for(int indice = i; indice < restantes; indice++)
                    ciudadesSV[indice] = NULL;
                restantes--;
                elimine = TRUE;
            }
        }
    }

    printf("Reducir horas: \n");
    for(int i = 1; i < cantCiudades; i++) {
        printf("%s \n", (*ciudadesV+i)->nombre);
    }
    printf("Distancia recorrida: %f \n", distanciaTotal);
}

void salir(){
    //liberar todo
    exit(0);
}

int main(int argc, char *argv[]){
    //se deberia leer el archivo en cada operacion 1|2|3, y se insertan las entradas en la ccp ahi
    //crear la funcion de prioridad en cada operacion tambien
    //hacer malloc para la cadena de char del nombre de la ciudad
    //ver el tema de $>planificador <archvo_texto>
    if(argc != 2)
        salir();
    int cantCiudades = 0;
    TCiudad ciudades = guardarCiudades(&cantCiudades, argv[1]);
    int termino = FALSE;
    do{

        printf("Ingrese el numero de la operacion que desee realizar: \n");
        printf("1. Mostrar las ciudades por visitar en orden ascendente\n");
        printf("2. Mostrar las ciudades por visitar en orden descendente\n");
        printf("3. Reducir horas de manejo\n");
        printf("4. Salir\n");
        int opcion = -1;
        printf("\n");
        scanf("%d", &opcion);
        printf("\n");
        switch(opcion){
            case 1: mostrarAscendente(ciudades, &cantCiudades); break;
            case 2: mostrarDescendente(ciudades, &cantCiudades); break;
            case 3: reducirHorasManejo(ciudades, &cantCiudades); break;
            case 4: salir(); termino = TRUE; break;
            default: printf("El numero ingresado no es valido\n"); fflush(stdin);break;
        }
        printf("\n");
    }while (!termino);
    return 0;
}


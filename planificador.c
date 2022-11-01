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
    for(i = 0; i < 50; i++)
        temp[i] = '\0';
}

TCiudad guardarCiudades(int *size) {
    int i; int j;
    int cont = 0;
    char temp[50];
    char aux;
    TCiudad ret = NULL;
    FILE * ptr;
    ptr = fopen("archivo_texto.txt","r");
    if(ptr != NULL) {
        while(!feof(ptr)) {             //cuento la cantidad de ciudades
            fgets(temp, 50, ptr);
            cont++;
        }
        *size = cont ;                  //guardo la longitud del arreglo
        rewind(ptr);                    //vuelvo al principio del archivo
        TCiudad ciudad;
        ciudad = (TCiudad)malloc(cont * sizeof(struct ciudad)); //
        if(ciudad == NULL)
            printf("No se pudo reservar memoria. \n");
        else {
            vaciar(temp);
            fgets(temp, 50, ptr);           //me salto la primera linea
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
                fgets(temp,7,ptr);           //lee el resto de la linea
                ciudad[i].pos_y = atof(temp);
            }
                //Leo y guardo la posicion
                rewind(ptr);                //vuelvo al principio del archivo
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

void mostrarAscendente(){
    //a lo ultimo llamar a destruir cola con una f que libere la memoria de la entrada
    int cantCiudades = 0; int i;
    TColaCP cola = crearColaCp(minHeap);
    TCiudad ciudad = guardarCiudades(&cantCiudades);
    for(i = 1; i < cantCiudades; i++) {
        float* distancia = (float*) malloc(sizeof(float));
        *distancia = calcularDistancia(ciudad[0].pos_x,ciudad[0].pos_y, ciudad[i].pos_x, ciudad[i].pos_y);
        TEntrada entrada = crearEntrada((TClave)&ciudad[i], (TValor)distancia);         //Entrada (TCiudad, Distancia)
        cpInsertar(cola, entrada);
        /*printf("%d. %s \n",i,((TCiudad)entrada->clave)->nombre);
        if(cola->raiz->hijo_izquierdo != NULL)
            printf("HIJO IZQ %s \n",((TCiudad)cola->raiz->hijo_izquierdo->entrada->clave)->nombre);*/

    }
       /* printf("raiz %s \n",((TCiudad)cola->raiz->entrada->clave)->nombre);
        if(cola->raiz->hijo_izquierdo != NULL)
        printf("raiz hi %s \n",((TCiudad)cola->raiz->hijo_izquierdo->entrada->clave)->nombre);
        if(cola->raiz->hijo_derecho != NULL)
        printf("raiz hd %s \n",((TCiudad)cola->raiz->hijo_derecho->entrada->clave)->nombre);
        if(cola->raiz->hijo_izquierdo->hijo_izquierdo != NULL)
        printf("raiz hi hi  %s \n",((TCiudad)cola->raiz->hijo_izquierdo->hijo_izquierdo->entrada->clave)->nombre);
        printf("raiz hi hd  %s \n",((TCiudad)cola->raiz->hijo_izquierdo->hijo_derecho->entrada->clave)->nombre);
        printf("raiz hd hi  %s \n",((TCiudad)cola->raiz->hijo_derecho->hijo_izquierdo->entrada->clave)->nombre);
        printf("raiz hd hd  %s \n",((TCiudad)cola->raiz->hijo_derecho->hijo_derecho->entrada->clave)->nombre);
        printf("\n");*/
    i = 1;
    while(cola->cantidad_elementos > 0) {
        TEntrada ent = cpEliminar(cola);
        printf("%d. %s \n",i,((TCiudad)ent->clave)->nombre);
        i++;
    }
    //free(entrada->valor);
    //free(cola);
}

void mostrarDescendente(){
    int cantCiudades = 0; int i;
    TColaCP cola = crearColaCp(maxHeap);
    TCiudad ciudad = guardarCiudades(&cantCiudades);
    for(i = 1; i < cantCiudades; i++) {
        float* distancia = (float*) malloc(sizeof(float));
        *distancia = calcularDistancia(ciudad[0].pos_x,ciudad[0].pos_y, ciudad[i].pos_x, ciudad[i].pos_y);
        TEntrada entrada = crearEntrada((TClave)&ciudad[i], (TValor)distancia);         //Entrada (TCiudad, Distancia)
        cpInsertar(cola, entrada);
    }
    i = 1;
    while(cola->cantidad_elementos > 0) {
        TEntrada ent = cpEliminar(cola);
        printf("%d. %s \n",i,((TCiudad)ent->clave)->nombre);
        i++;
    }
    //free(entrada->valor)
    //free(cola);

}

void reducirHorasManejo(){
    //falta liberar memoria
    int cantCiudades; //tenes q hace -1 en algun lado pelotudo
    float restantes;
    float * distanciaTotal = (float*)malloc(sizeof(float));
    int i; int elimine;
    int j = 1;
    *distanciaTotal = 0;                                    //Se guarda en la pos_x de la primera ciudad de ciudades visitadas
    TCiudad ciudadesSV = guardarCiudades(&cantCiudades);    //ciudades sin visitar
    TCiudad ciudadesV = (TCiudad)malloc(cantCiudades * sizeof(struct ciudad));      //ciudades visitadas
    restantes = cantCiudades;                               //Controlo la cantidad de ciudades sin visitar
    //guardo la distancia recorria en la primer ciudad
    ciudadesV[0].nombre = "Total";
    ciudadesV[0].pos_x = *distanciaTotal;
    while(restantes > 1) {                                 //mientras queden ciudades sin visitar
        TColaCP cola = crearColaCp(minHeap);
        for(i = 1; i < cantCiudades; i++) {
            float* distancia = (float*) malloc(sizeof(float));
            *distancia = calcularDistancia(ciudadesSV[0].pos_x,ciudadesSV[0].pos_y, ciudadesSV[i].pos_x, ciudadesSV[i].pos_y);
            TEntrada entrada = crearEntrada((TClave)&ciudadesSV[i], (TValor)distancia);         //Entrada (TCiudad, Distancia)
            cpInsertar(cola, entrada);
        }
        TEntrada ent = cpEliminar(cola);
        //actualizo la posicion
        ciudadesSV[0].pos_x = ((TCiudad)ent->clave)->pos_x;
        ciudadesSV[0].pos_y = ((TCiudad)ent->clave)->pos_y;
        printf("x: %f\n", ((TCiudad)ent->clave)->pos_x);

        printf("y: %f\n", ((TCiudad)ent->clave)->pos_y);

        ciudadesV[j] = *((TCiudad)ent->clave);
        ciudadesV[0].pos_x += *(float*)ent->valor;          //aumento la distancia recorrida
        j++;
        //elimino la ciudad de la lista de no visitadas
        elimine = 0;
        i = 1;
        while(i < restantes && !elimine ) {
            if(&ciudadesSV[i] == ((TCiudad)ent->clave)) {
                int indice;
                for(indice = i; indice < restantes; indice++)
                    ciudadesSV[indice] = ciudadesSV[indice+1];
                restantes--;
                elimine = 1;
            }
            i++;
        }
    //fin del while
    }
    for(i = 1; i < cantCiudades; i++) {
        printf("%s \n", ciudadesV[i].nombre);
    }
    printf("Distancia recorrida: %f \n", ciudadesV[0].pos_x);
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
            case 1: mostrarAscendente(); break;
            case 2: mostrarDescendente(); break;
            case 3: reducirHorasManejo(); break;
            case 4: salir(); termino = TRUE; break;
            default: printf("El numero ingresado no es valido\n"); fflush(stdin);break;
        }
        printf("\n");
    }while (!termino);
    return 0;
}


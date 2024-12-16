#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>

void* encontrar_maximo(void* arg) {
    HebraData* data = (HebraData*)arg;
    int* arreglo = data->arreglo;
    int tamano = data->tamano;
    int indice = data->indice;
    int* cambios = data->cambios;

    int myval = arreglo[indice];

    while (1) {
        int indice_anterior = (indice - 1 + tamano) % tamano;

        pthread_mutex_lock(data->mutex);

        /*
        printf("Hebra %d: A[%d] = %d, A[%d] = %d\n",
               indice, indice, arreglo[indice], indice_anterior, arreglo[indice_anterior]);*/

        // Comparar y actualizar si es necesario
        if (arreglo[indice_anterior] > myval) {
            myval = arreglo[indice_anterior];
            arreglo[indice] = myval;
            *cambios = 1; // Marcar que hubo un cambio
        }

        pthread_mutex_unlock(data->mutex);
        return NULL;
    }
}

// Leer archivo de entrada
int leer_archivo(const char* nombre_archivo, int** arreglo, int* tamano) {
    FILE* archivo = fopen(nombre_archivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo de entrada");
        return -1;
    }

    int capacidad = 10;
    *arreglo = malloc(capacidad * sizeof(int));
    if (!*arreglo) {
        perror("Error al asignar memoria para el arreglo");
        fclose(archivo);
        return -1;
    }

    int count = 0;
    int temp;
    while (fscanf(archivo, "%d", &temp) != EOF) {
        if (count == capacidad) {
            capacidad *= 2;
            int* nuevo_arreglo = realloc(*arreglo, capacidad * sizeof(int));
            if (!nuevo_arreglo) {
                perror("Error al redimensionar el arreglo");
                free(*arreglo);
                fclose(archivo);
                return -1;
            }
            *arreglo = nuevo_arreglo;
        }
        (*arreglo)[count++] = temp;
    }

    *tamano = count;
    fclose(archivo);
    return 0;
}


// Escribir archivo de salida
void escribir_archivo(const char* nombre_archivo, int* arreglo, int tamano) {
    FILE* archivo = fopen(nombre_archivo, "w");
    if (!archivo) {
        fprintf(stderr, "Error al abrir el archivo de salida: %s\n", nombre_archivo);
        return;
    }

    for (int i = 0; i < tamano; i++) {
        if (fprintf(archivo, "%d\n", arreglo[i]) < 0) {
            fprintf(stderr, "Error al escribir en el archivo de salida\n");
            fclose(archivo);
            return;
        }
    }

    fclose(archivo);
}

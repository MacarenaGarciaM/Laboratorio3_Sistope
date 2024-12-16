#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <pthread.h>

// Estructura para datos de hebras
typedef struct {
    int* arreglo;           // Puntero al arreglo compartido
    int tamano;             // Tamaño del arreglo
    int indice;             // Índice de la hebra
    int* cambios;           // Puntero al indicador de cambios global
    pthread_mutex_t* mutex; // Mutex para exclusión mutua
} HebraData;

// Prototipos de funciones
void* encontrar_maximo(void* arg);
int leer_archivo(const char* nombre_archivo, int** arreglo, int* tamano);
void escribir_archivo(const char* nombre_archivo, int* arreglo, int tamano);

#endif

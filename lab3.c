#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>

int main(int argc, char* argv[]) {
    char *input_file = NULL, *output_file = NULL;
    int mostrar_arreglo = 0;

    // Procesar argumentos de línea de comandos
    int opt;
    while ((opt = getopt(argc, argv, "i:o:D")) != -1) {
        switch (opt) {
            case 'i': input_file = optarg; break;
            case 'o': output_file = optarg; break;
            case 'D': mostrar_arreglo = 1; break;
            default:
                fprintf(stderr, "Uso: %s -i input.txt -o output.txt -D\n", argv[0]);
                return EXIT_FAILURE;
        }
    }

    if (!input_file || !output_file) {
        fprintf(stderr, "Error: Los archivos de entrada y salida son obligatorios\n");
        return EXIT_FAILURE;
    }

    // Leer arreglo desde el archivo
    int* arreglo;
    int tamano;
    if (leer_archivo(input_file, &arreglo, &tamano) < 0) {
        return EXIT_FAILURE;
    }

    // Inicializar hebras y mutex
    pthread_t* threads = malloc(tamano * sizeof(pthread_t));
    HebraData* datos = malloc(tamano * sizeof(HebraData));
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int cambios = 1;

    // Crear un grupo fijo de hebras
    for (int i = 0; i < tamano; i++) {
        datos[i] = (HebraData){arreglo, tamano, i, &cambios, &mutex};
        pthread_create(&threads[i], NULL, encontrar_maximo, &datos[i]);
    }

    
while (cambios) {
    cambios = 0; // Reiniciar el contador de cambios antes de la iteración
    for (int i = 0; i < tamano; i++) {
        datos[i] = (HebraData){arreglo, tamano, i, &cambios, &mutex};
        pthread_create(&threads[i], NULL, encontrar_maximo, &datos[i]);
    }

    // Esperar a todas las hebras
    for (int i = 0; i < tamano; i++) {
        pthread_join(threads[i], NULL);
    }
}

    // Escribir el resultado al archivo de salida
    escribir_archivo(output_file, arreglo, tamano);

    // Mostrar el arreglo si se especificó el flag -D
    if (mostrar_arreglo) {
        for (int i = 0; i < tamano; i++) {
            printf("%d\n", arreglo[i]);
        }
    }

    // Liberar recursos
    free(arreglo);
    free(threads);
    free(datos);
    pthread_mutex_destroy(&mutex);

    return EXIT_SUCCESS;
}

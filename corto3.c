#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

// Estructura para pasar los argumentos a cada hilo
typedef struct {
    int *sublista;
    int inicio;
    int fin;
    long suma_parcial;
} argumentos_t;

// Función que cada hilo ejecuta para sumar los elementos de una sublista
void *sumar_sublista(void *args) {
    argumentos_t *argumentos = (argumentos_t *)args;
    long suma = 0;
    for (int i = argumentos->inicio; i < argumentos->fin; i++) {
        suma += argumentos->sublista[i];
    }
    argumentos->suma_parcial = suma;
    pthread_exit(NULL);
}

int fork_join_suma(int *lista, int longitud, int num_hilos) {
    pthread_t hilos[num_hilos];
    argumentos_t argumentos[num_hilos];
    int chunk_size = longitud / num_hilos;
    int suma_total = 0;

    // Crear y lanzar los hilos
    for (int i = 0; i < num_hilos; i++) {
        argumentos[i].sublista = lista;
        argumentos[i].inicio = i * chunk_size;
        argumentos[i].fin = (i == num_hilos - 1) ? longitud : (i + 1) * chunk_size;  // El último hilo toma los elementos restantes
        argumentos[i].suma_parcial = 0;

        // Crear hilo para procesar la sublista
        pthread_create(&hilos[i], NULL, sumar_sublista, (void *)&argumentos[i]);
    }

    // Esperar a que todos los hilos terminen y sumar los resultados parciales
    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
        suma_total += argumentos[i].suma_parcial;
    }

    return suma_total;
}

int main() {
    int longitud = 1000;
    int lista[longitud];

    // Llenar la lista con números aleatorios
    for (int i = 0; i < longitud; i++) {
        lista[i] = rand() % 100 + 1;
    }
    

    int num_hilos = NUM_THREADS;
    int resultado = fork_join_suma(lista, longitud, num_hilos);
    printf("La suma total es: %d\n", resultado);

    return 0;
}
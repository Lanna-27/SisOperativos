#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

#define NUM_HILOS 16
#define MAXITER 1E05

struct datos{
    int a;
    double res;
};

void * function(void *datos){
    clock_t tiempo_inicio, tiempo_final;
    tiempo_inicio = clock();
    struct datos *datosH;
    datosH = datos;
    double parcial = 0;
    int i = datosH->a;
    for (int j = (i * MAXITER); j < ((i + 1) * MAXITER); j++) { 
        parcial = parcial + (double)((j % 2 == 0) ? 4 : -4) / (2.0 * j + 1.0);
    }

    datosH->res = parcial;
    tiempo_final = clock(); 
    printf("parte de la sumatoria de leibniz en hilo %i: %f\n", i, parcial);
    printf("Tiempo de ejecución en hilo %i: %.30f\n", i, ((double)(tiempo_final- tiempo_inicio) / CLOCKS_PER_SEC));
    fflush(stdout); 
}

int main(){

    pthread_t hilo[NUM_HILOS];
    struct datos datosh[NUM_HILOS];
    int r = 0, *rh0, i;
 
    clock_t tiempo_inicio, tiempo_final;

    tiempo_inicio = clock();

    for(i = 0; i < NUM_HILOS; i++){
        datosh[i].a = i;
        datosh[i].res = 0;
    }

    for (i = 0; i < NUM_HILOS; i++)
    {
        r = pthread_create(&hilo[i], NULL, (void *)function, (void *)&datosh[i]);
        if(r != 0){
            perror("\n-->pthread_create error: ");
            exit(-1);
        }
    }
    for (i = 0; i < NUM_HILOS; i++)
    {
        pthread_join(hilo[i], (void **)&rh0);
        if (r != 0)
        {
            perror("\n-->pthread_join error: ");
            exit(-1);
        }
    }

    double pi = 0.0;
    for (i = 0; i < NUM_HILOS; i++)
    {
        pi += datosh[i].res;
    }
    printf("Aproximación de PI: %.30f\n", pi);
    tiempo_final = clock();
    printf("Tiempo de ejecución: %.30f\n", ((double)(tiempo_final- tiempo_inicio) / CLOCKS_PER_SEC));
    return 0;   
    
}
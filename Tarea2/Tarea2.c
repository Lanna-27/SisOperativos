#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROCESOS 4
#define ITERACIONES 1000000

int main() {
    int i, j, pid, status;
    int fd[2];
    double resultado = 0.0;

    if (pipe(fd) == -1) {
        perror("pipe");
        exit(1);
    }

    for (i = 0; i < NUM_PROCESOS; i++) {
        pid = fork();
        if (pid == -1) {
            perror("Error al crear proceso hijo");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            double parcial = 0.0;
            for (j = i * ITERACIONES; j < (i + 1) * ITERACIONES; j++) {
                parcial += (double)((j % 2 == 0) ? 1 : -1) / (2.0 * j + 1.0);
            }
            printf("Proceso %d terminado con resultado: %.15f\n", i, parcial);
            exit((int)(parcial * 1E9));
        }
    }
    
    for (i = 0; i < NUM_PROCESOS; i++) {
        pid = waitpid(-1, &status, 0);
        if (WIFEXITED(status)) {
            printf("%d", WEXITSTATUS(status));
            resultado += (double)WEXITSTATUS(status) / 1E9;
        } else {
            printf("Proceso %d ha fallado\n", i);
        }
    }
    
    resultado *= 4.0;
    
    printf("Valor de pi aproximado: %f\n", resultado);
    
    return 0;
}

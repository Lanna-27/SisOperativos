#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROCESOS 4
#define ITERACIONES 1000000

int main() {
    int i, j, pid, status;
    int pipefd[2];
    double resultado = 0.0;

    if (pipe(pipefd) == -1) {
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
            itoa(parcial,string,10);
            exit(EXIT_SUCCESS);
        }
    }
    
    for (i = 0; i < NUM_PROCESOS; i++) {
        wait(&status);
        if (WIFEXITED(status)) {

            close(pipefd[1]);
            r = read(pipefd[0], buf, 10);
            buf[r] = 0;
            close(pipefd[0]);

            resultado += WEXITSTATUS(status);
        } else {
            printf("Proceso %d ha fallado\n", i);
        }
    }
    
    resultado *= 4.0;
    
    printf("Valor de pi aproximado: %f\n", resultado);
    
    return 0;
}

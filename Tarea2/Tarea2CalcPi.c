#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROCESOS 8
#define ITERACIONES 1E05

int main() {
    pid_t pid;
    int i, j, status, r;
    int pipefd[2];
    double parcial = 0;
    double res = 0;

    // r = pipe(pipefd);
    // if (r < 0) {
    //     perror("Error pipe()");
    //     exit(-1);
    // }

    for (i = 0; i < NUM_PROCESOS; i++) {
        r = pipe(pipefd);
        if (r < 0) {
            perror("Error pipe()");
            exit(-1);
        }
        pid = fork();
        if (pid < 0) {
            perror("Error al crear proceso hijo");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            //close(pipefd[1]);
            //read(pipefd[0], &resultado, sizeof(resultado));
            double parcial = 0;
            for (j = i * ITERACIONES; j < (i + 1) * ITERACIONES; j++) {
                parcial += (double)((j % 2 == 0) ? 1 : -1) / (2.0 * j + 1.0);
            }
            printf("Proceso %d terminado con resultado: %.15f\n", i, parcial);

            //res = res + parcial;
            printf("Res: %.15f\n", res);
            close(pipefd[0]);
            write(pipefd[1], &parcial, sizeof(double));
            close(pipefd[1]);

            exit(EXIT_SUCCESS);
        }else{
            wait(&status);
            double parcial_hijo = 0;
            if (WIFEXITED(status)) {
                //double parcial_hijo = 0;
                close(pipefd[1]);
                read(pipefd[0], &parcial_hijo, sizeof(double));
                close(pipefd[0]);

            } else {
                printf("Proceso %d ha fallado\n", i);
            }
            res += parcial_hijo;
        }
    }

    res *= 4.0;
    
    printf("Valor de pi aproximado: %.15f\n", res);
    
    return 0;    
}
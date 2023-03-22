//Ejemplo procesos con memoria compartida
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
  pid_t pid;
  key_t key=1234;
  int shmId;
  float a, b, c, *ap;
  int status, cpid, r;

  shmId = shmget(key, 3*sizeof(float), 0666|IPC_CREAT); //el num es sobre los permisos de lectura y escritura de la mem
  if(shmId < 0){
    perror("Error en shmget");
    exit(-1);
  }
  ap = (float *)shmat(shmId, 0, 0); //0,0 -> la memoria no tiene direccion especifica, ni bandera
  if(ap < 0)
  {
    perror("Error en shmat");
    exit(-1);
  }
  *ap = 3.1415926535;

  pid = fork();
  if(pid < 0)
  {
    perror("Error fork");
    exit(-1);
  }
  if(pid == 0){ //hijo
    *(ap+1)= 0.707;
  }else{ //padre
    if(wait(&status) == pid){
    printf("\n %f    %f\n", *ap, *(ap+1));
    }
  }
  r = shmdt(ap);
  if (r < 0)
  {
    perror("error shmdt ");
    exit(-1);
  }

  return 0;  
}
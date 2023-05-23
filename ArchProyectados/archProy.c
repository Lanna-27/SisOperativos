#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define SIZE 200
/* Offset del inicio de la parte a leer*/
#define OFFSETR 80 
/* Offset del inicio de la parte a escribir*/
#define OFFSETW 150

int main() {
  int fd, tam_fich;
  struct stat sb;
  char *file_data, *buff;
  off_t offsetRead = OFFSETR;
  off_t offsetWrite = OFFSETW;
  size_t size = SIZE;    // Tamaño de la parte a leer

  // Abrir el archivo en modo lectura y escritura
  fd = open("archivo.txt", O_RDWR);
  if (fd == -1) {
    perror("open");
    exit(1);
  }
  // Proyectar el archivo en memoria
  // void * mmap (void *address, size_t length, int protect, int flags, int filedes, off_t offset);

  tam_fich = lseek (fd, 0, SEEK_END); //encontrar el tamaño del archivo
  file_data = mmap(NULL, tam_fich, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (file_data == MAP_FAILED) {
    perror("mmap");
    exit(1);
  }

  // Cerrar el descriptor de archivo, ya que no lo necesitamos más
  close(fd);
  
  //  Escribir "todas los 1s como as" en el archivo proyectado en memoria    
  for (int i=0; i<tam_fich; i++ ){
    if (file_data[i]=='1') file_data[i]='a';
  }
    
  // Leer una parte del archivo proyectado
  printf("Parte aleatoria del archivo proyectado:\n%.*s\n\n", (int)size,
         file_data + offsetRead);

  //Hacer una búsqueda dentro del archivo, cuantos 9s hay en el archivo?

  int total9 = 0;
  for (int i=0; i<tam_fich; i++ ){
    if(file_data[i] == '9'){
      total9 += 1; 
    }   
  }

  printf("Hay %d 9s dentro del archivo\n", total9);
  // Liberar el archivo proyectado en memoria
  if (munmap(file_data, sb.st_size) == -1) {
    perror("munmap");
    exit(1);
  }

  return 0;
}

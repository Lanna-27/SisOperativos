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

/*
struct stat {
               dev_t     st_dev;         // ID of device containing file
               ino_t     st_ino;         // Inode number
               mode_t    st_mode;        // File type and mode
               nlink_t   st_nlink;       // Number of hard links
               uid_t     st_uid;         // User ID of owner
               gid_t     st_gid;         // Group ID of owner
               dev_t     st_rdev;        // Device ID (if special file)
               off_t     st_size;        // Total size, in bytes
               blksize_t st_blksize;     // Block size for filesystem I/O
               blkcnt_t  st_blocks;      // Number of 512B blocks allocated

               // Since Linux 2.6, the kernel supports nanosecond
                  precision for the following timestamp fields.
                  For the details before Linux 2.6, see NOTES.

               struct timespec st_atim;  // Time of last access
               struct timespec st_mtim;  // Time of last modification
               struct timespec st_ctim;  // Time of last status change

           #define st_atime st_atim.tv_sec      // Backward compatibility
           #define st_mtime st_mtim.tv_sec
           #define st_ctime st_ctim.tv_sec
           };
*/

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

  // Obtener información sobre el archivo en una estructura tipo stat
  if (fstat(fd, &sb) == -1) {
    perror("fstat");
    exit(1);
  }

  // Proyectar el archivo en memoria
  // void * mmap (void *address, size_t length, int protect, int flags, int filedes, off_t offset);

  tam_fich = lseek (fd, 0, SEEK_END); //encontrar el tamaño del archivo
  file_data = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
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

//Hecho con chatGPT
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define FILE_LENGTH 0x100

int main(int argc, char* const argv[]) {
    int fd, count;
    char* file_memory;
    char str[] = "Hello, world!\n";
    struct stat statbuf;

    // Abrir el archivo para escritura
    fd = open("archivo.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Obtener el tamaño del archivo
    if (fstat(fd, &statbuf) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    // Proyectar el archivo en memoria
    file_memory = mmap(0, FILE_LENGTH, PROT_WRITE, MAP_SHARED, fd, 0);
    if (file_memory == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    // Escribir datos en el archivo proyectado
    for (count = 0; count < sizeof(str); ++count) {
        file_memory[count] = str[count];
    }

    // Desproyectar el archivo en memoria
    if (munmap(file_memory, FILE_LENGTH) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    // Cerrar el archivo
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_LENGHT 256
static char recieve[BUFFER_LENGHT];

int main(){
    int ret, fd;
    char numToSend[BUFFER_LENGHT];
    printf("killDriver test...\n");
    fd = open("/dev/killDriver", 0_RDWR); //Llama al driver para poder usarlo.
    if(fd < 0){
        perror("Failed to open the device...");
        return errno;
    }
    printf("Type in a number to send to the kernel module:\n");
    scanf("%[^\n]%*c", numToSend);
    printf("Writing number to the device [%s].\n", numToSend);
    ret = write(fd, numToSend, strlen(numToSend));
    if(ret < 0){
        perror("Failed to write the number to the device.");
        return errno;
    }

    printf("Press ENTER to read back from the device...\n");
    getchar();

    printf("Reading from the device...\n");
    ret = read(fd, recieve, BUFFER_LENGHT);
    if(ret < 0){
        perror("Failed to read the message to the device.");
        return errno;
    }

    printf("The name of the process related is: [%s]\n", recieve);
    printf("End of the program\n");
    return 0;
}
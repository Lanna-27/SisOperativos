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
    char stringToSend[BUFFER_LENGHT];
    printf("chatDriver test...\n");
    fd = open("/dev/charDriver", 0_RDWR); //Llama al driver para poder usarlo.
    if(fd < 0){
        perror("Failed to open the device...");
        return errno;
    }
    printf("Type in a short string to send to the kernel module:\n");
    scanf("%[^\n]%*c", stringToSend);
    printf("Writing message to the device [%s].\n", stringToSend);
    ret = write(fd, stringToSend, strlen(stringToSend));
    if(ret < 0){
        perror("Failed to write the message to the device.");
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

    printf("The recieved message is: [%s]\n", recieve);
    printf("End of the program\n");
    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#define BUFFER_LENGHT 256
static char recieve[BUFFER_LENGHT];

int main(){
    pid_t pid;
    int ret, fd, r;
    int pipefd[2];
    char numToSend[BUFFER_LENGHT];
    printf("killDriver test...\n");
    fd = open("/dev/killDriver", 0_RDWR); //Llama al driver para poder usarlo.
    if(fd < 0){
        perror("Failed to open the device...");
        return errno;
    }
    
    r = pipe(pipefd);
    if(pid < 0){
        perror("Error pipe()");
        exit(-1);
    }
    else if(pid == 0){
        //Hijo 
        close(pipefd[0]);
        sprintf(numToSend,"%d", getpid());
        write(pipefd[1], numToSend, 20);
        close(pipefd[1]);
        exit(0);
    }
    else{
        close(pipefd[1]);
        r = read(pipefd[0],numToSend, 20);
        numToSend[r] = 0;
        close(pipefd[0]);
    
        //printf("Type in a number to send to the kernel module:\n");
        //scanf("%[^\n]%*c", numToSend);
        //printf("Writing number to the device [%s].\n", numToSend);
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
}
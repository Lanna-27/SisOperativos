#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    FILE *archtxt, *archbin;

    archtxt = fopen("archivo.txt", "r");
    archbin = fopen("archivo.dat", "rb");

    if(archtxt != NULL && archbin != NULL){
        printf("Los archivos se abrieron correctamente\n");

        char lineatxt[200];
        char lineabin[200];
        fgets(lineatxt, 200, archtxt);
        fgets(lineabin, 200, archbin);

        printf("\nLos datos de la persona son (txt): %s\n", lineatxt);
        printf("\nLos datos de la persona son (bin): %s\n", lineabin);

    } else{
        exit(0);
    }

    fclose(archtxt);
    fclose(archbin); 
}
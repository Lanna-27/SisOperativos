#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    FILE *archtxt, *archbin;

    archtxt = fopen("archivo.txt", "w");
    archbin = fopen("archivo.dat", "wb");

    struct persona {
        char nombre[20];
        int edad;
        float estatura;
    };

    struct persona p1;
    strcpy(p1.nombre, "Laura"); 
    p1.edad = 20;
    p1.estatura = 1.65;

    if(archtxt != NULL && archbin != NULL){
        printf("Los archivos se crearon correctamente\n");

        fprintf(archtxt, "%s", p1.nombre);
        fprintf(archtxt,", %i", p1.edad);
        fprintf(archtxt,", %1.3f", p1.estatura);

        fprintf(archbin, "%s", p1.nombre);
        fprintf(archbin,", %i", p1.edad);
        fprintf(archbin,", %1.3f", p1.estatura);
    }else{
        exit(0);
    } 

    fclose(archtxt);
    fclose(archbin);  

    // printf("\n Persona 1");
	// printf("\n Nombre: %s ", p1.nombre);
	// printf("\n Edad: %i ", p1.edad);	
	// printf("\n Estatura: %1.3f ", p1.estatura);
}

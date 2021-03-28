#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char*argv[]){

    char *palabra = (char*)malloc(2100*sizeof(char)); //Para reservar memoria, palabra va ser un apuntador
    //char* para poder luego hacer aritmetica
    //char palabra[2100]; pero no estariamos reservando
    if(argc < 2){
        printf("You must specify a filepath\n");
        return EXIT_FAILURE;
    }
    //Apuntador
    FILE* fp = fopen(argv[1], "r");

    if(!fp){
        printf("Error opening the file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    //Vamos a leer el archivo linea por linea
    char line[1024]; //Si la linea mide más de esto, solo toma lo que quepa
    //char *rest = line; // * es un apuntador

    int linecount = 0;
    int quantities[4];
    int termscount = 0;
    int numingredients = 0;
    int numOfDifferentIngredients = 0;
    char *nullchar = '\0';

    while(fgets(line, 1024, fp)){
        //printf("Linea: %s\n", line);
        //Vamos a procesar la linea primer linea
        char * token;
        char *rest = line; 
        if(linecount == 0){
            while((token = strtok_r(rest, " ", &rest))){
                quantities[termscount] = atoi(token); 
                termscount++;
            } 
            linecount++;
        }else{
            token = strtok_r(rest, " ", &rest);
            numingredients = atoi(token);
            for(int i=0; i<numingredients; i++){ 
                token = strtok_r(rest, " ", &rest);
                if(i == numingredients - 1){
                    char *s = token; 

                    while(*s != '\n'){ 
                        ++s;
                    }
                    *s = '\0'; 
                }

                int comparisonSomeIsEqual = -1; 
                for(int j=0; j<numOfDifferentIngredients; j++){
                    if(strcmp((palabra+(j*21)), token) == 0){ 
                        comparisonSomeIsEqual = 0; 
                        break;
                    }
                }

                if(comparisonSomeIsEqual == -1){
                    strcpy((palabra+(numOfDifferentIngredients*21)), token);
                    numOfDifferentIngredients++;
                }
            }
        }    
    }

    fclose(fp);  

    for(int i = 0; i<4; i++){
        printf("Quantity: %d\n", quantities[i]); 
    }

    for(int i=0; i<numOfDifferentIngredients; i++){
        printf("ingrediente: %s\n", (palabra + (i*21)) + 0 );
    }

    printf("Número total diferente de ingredientes: %d \n", (numOfDifferentIngredients));

    return EXIT_SUCCESS;
    
}
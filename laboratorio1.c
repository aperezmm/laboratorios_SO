#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char*argv[]){

    //----------------------------------LEEMOS ARCHIVO----------------------------------
    char *palabra = (char*)malloc(2100*sizeof(char)); 
    if(argc < 2){
        printf("You must specify a filepath\n");
        return EXIT_FAILURE;
    }
    FILE* fp = fopen(argv[1], "r");

    if(!fp){
        printf("Error opening the file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    //----------------------------------FIN LEEMOS ARCHIVO----------------------------------

    char line[1024]; 

    int linecount = 0;
    int quantities[4];
    int termscount = 0;
    int PP = 0;
    int P2 = 0;
    int P3 = 0;
    int P4 = 0;
    int NTPP = 0;
    int numingredients = 0;
    int numOfDifferentIngredients = 0;
    char *nullchar = '\0';

    //----------------------------------RECORREMOS ARCHIVO----------------------------------
    while(fgets(line, 1024, fp))
    {
        //printf("Linea: %s\n", line);
        char* token;
        char* rest = line; 
        //strtok_r()
        if(linecount == 0){
            while((token = strtok_r(rest, " ", &rest))){
                quantities[termscount] = atoi(token); 
                termscount++;
                
            }
            linecount++;
            PP = quantities[0];
            P2 = quantities[1];
            P3 = quantities[2];
            P4 = quantities[3];
            NTPP = P2 * 2 + P3 * 3 + P4 * 4;
        }else{
            //----------------------------------PROCESO CADA LINEA----------------------------------
            token = strtok_r(rest, " ", &rest);
            //printf("Que contiene token _: %s\n", token);
            numingredients = atoi(token);
            for(int i=0; i<numingredients; i++){ //Asumimos que me mandaron tantos ingredientes como el primer num
                token = strtok_r(rest, " ", &rest);
                //printf("Que contiene token2 _: %s\n", token);
                //printf("Read token: %s\n", token); 

                if(i == numingredients - 1){
                    char *s = token; 
                    while(*s != '\n'){ 
                        //printf("%s\n", s);
                        ++s;
                    }
                    *s = '\0';
                    break;
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
                    //Va separando de a 21 caracteres
                    numOfDifferentIngredients++;
                }
            }
        }
        //----------------------------------FIN PROCESO CADA LINEA----------------------------------    
    }

    //------------------------------RESERVAMOS ESPACIO PARA MATRIZ---------------------------------
    int **P;
    P = (int**)malloc(sizeof(int*)*numOfDifferentIngredients);
    for(int i=0; i<numOfDifferentIngredients; i++){
        P[i] = (int*)malloc(sizeof(int)*PP);
    }
    //------------------------------FIN RESERVAMOS ESPACIO PARA MATRIZ---------------------------

    //------------------------------LLENAMOS MATRIZ CON CEROS------------------------------------
    for(int i=0; i<numOfDifferentIngredients; i++){
        for(int j=0; j<PP; j++){
            P[i][j] = 0;
        }
    }
    //------------------------------FIN LLENAMOS MATRIZ CON CEROS-------------------------------

    fclose(fp);
    //----------------------------------FIN RECORREMOS ARCHIVO----------------------------------

    //---------------------SEGUNDA LECTURA ARCHIVO PARA LLENAR MATRIZ---------------------------
    FILE *nfp = fopen(argv[1], "r");
    int count = 0;
    char line2[1024];
    int linecounter = 0;
    int numingredientes = 0;
    int aux = 0;
    int aux2 = 0;

    while(fgets(line2, 1024, nfp)){
        printf("Linea: %s\n", line2);
        char* token2;  
        char* rest2 = line2; 

        if(linecounter == 0){
            //printf("Esta es la primer linea de la segunda leída. \n");
            linecounter++;
        }else{
            token2 = strtok_r(rest2, " ", &rest2);
            //printf("Que contiene token _: %s\n", token2);
            numingredientes = atoi(token2);
            for(int i=0; i<numingredientes; i++){
                token2 = strtok_r(rest2, " ", &rest2); 
                
                //printf("Rest : %s\n", rest2);

                if(i == numingredientes - 1){
                    char *s2 = token2; 
                    while(*s2 != '\n'){
                        ++s2;
                    }
                    *s2 = '\0';
                    break;
                }

                int comparisonIsExist = -1;
                
                //printf("El ingrediente : %s\n", token2);

                for(int i=0; i<numOfDifferentIngredients; i++){
                    if(strcmp(palabra+(i*21), token2) == 0){
                        printf("Palabra: %s\n", (palabra+(i*21)) + 0 );
                        printf("Token : %s\n", token2);
                        aux = i; //Contiene la posición del ingrediente x en la lista
                        break;
                    }
                    /* palabra = [
                        champi
                        cebolla
                        pimenton
                        jamon
                        huevo
                        cilantro
                        aji
                        ajo
                        queso
                        garbanzo
                    ]                        
                    */
                }

                P[aux][aux2] = 1;
                aux2++;
                if(aux2 == 10){
                    aux2 = 0;
                }
            }
        }
    }

    fclose(nfp);
    //---------------------FIN SEGUNDA LECTURA ARCHIVO PARA LLENAR MATRIZ---------------------------


    // INFORMACIÓN RELEVANTE PARA EL USUARIO
    if(NTPP != PP){
        printf("La cantidad de platos ingresada no coincide con lo que se estima. Revisa el archivo! \n");
        return EXIT_FAILURE;
    }

    printf("PP (Cantidad de platos pedidos) = %d\n", PP);
    printf("P2 (Cantidad de pedidos de dos platos) = %d\n", P2);
    printf("P3 (Cantidad de pedidos de tres platos) = %d\n", P3);
    printf("P2 (Cantidad de pedidos de cuatro platos) = %d\n", P4);
    printf("NTPP (Número total de platos) = %d\n", NTPP);
    printf("I (Cantidad de ingredientes total que no se repiten) = %d\n", numOfDifferentIngredients);    
    printf("Lista de ingredientes que no se repiten: \n");
    for(int i=0; i<numOfDifferentIngredients; i++){
        printf("%s\n", (palabra + (i*21)) + 0 ); 
    }

    printf("Matriz inicial vacía en ceros, con espacio reservado en memoria = ");
    for(int i=0; i<numOfDifferentIngredients; i++){
        printf("\n");
        for(int j=0; j<PP; j++){
            printf("%d", P[i][j]);
        }
    }
    printf("\n");   


    return EXIT_SUCCESS;
    
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//REVISAR PORQUE *WORDS[] PRODUCE UN WARNING
int verifyIngredientsQuantity(int quantity, char words[] ){
    //1 para OK
    //0 para error
    /*
    printf("quantity %d\n", quantity);
    printf("words %s\n", words);*/

    return 1;
}

void viewIngredients(int start, int interations){
    
}   

int main(int argc, char*argv[]){

    char *ingredients = (char*)malloc(2100*sizeof(char)); //Para reservar memoria, ingredients va ser un apuntador

    //char* para poder luego hacer aritmetica
    //char ingredients[2100]; pero no estariamos reservando
    if(argc < 2){
        fprintf("You must specify a filepath\n");
        return EXIT_FAILURE;
    } 
    //Apuntador
    FILE* fileRef = fopen(argv[1], "r");

    if(!fileRef){
        fprintf("Error opening the file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    //Vamos a leer el archivo linea por linea
    char line[1024]; //Si la linea mide más de esto, solo toma lo que quepa
    //char *rest = line; // * es un apuntador

    int linecount = 0;
    int quantities[4];
    int termscount = 0;
    int PP = 0;
    int P2 = 0;
    int P3 = 0;
    int P4 = 0;
    int NTPP = 0;
    //Para el else
    int numingredients = 0;
    int numOfDifferentIngredients = 0;
    char *nullchar = '\0';

    /*
    SE OBTIENE LA CANTIDAD DE PLATOS PEDIDOS, LA CANTIDAD DE INGREDIENTES
    DIFERENTES, CANTIDAD DE PEDIDOS DE DOS, TRES Y CUATRO PLATOS, EL NÚMERO
    TOTAL DE PLATOS PEDIDOS Y SE OBTIENE LOS INGREDIENTES DIFERENTES.
    */
    while(fgets(line, 1024, fileRef))
    {
        //printf("Linea: %s\n", line);
        //Vamos a procesar la linea
        char* token;
        char* rest = line; //contiene la linea leida
        //strtok_r()
        if(linecount == 0){
            while((token = strtok_r(rest, " ", &rest))){
                quantities[termscount] = atoi(token); //Lo convierto a un entero
                termscount++;
                
            } //Leame rest, partala donde encuentre espacio en blanco y lo
            //que quede llevemelo a rest
            linecount++;
            PP = quantities[0];
            P2 = quantities[1];
            P3 = quantities[2];
            P4 = quantities[3];
            NTPP = P2 * 2 + P3 * 3 + P4 * 4;
        }
        else{
            //Continuamos con la segunda linea, lectura de las lineas después de la primera
            token = strtok_r(rest, " ", &rest);
            //printf("Que contiene token _: %s\n", token);
            numingredients = atoi(token);

            //TO DO: Verificar que el numingredients sea realmente el 
            //numero de palabras que tiene la linea
            int result = verifyIngredientsQuantity(numingredients, rest);
            if( result == 0){
                return EXIT_FAILURE;
            }

            //LEEMOS CADA UNO DE LOS INGREDIENTES DE LA LINEA
            for(int i=0; i<numingredients; i++){
                token = strtok_r(rest, " ", &rest);
                //printf("Que contiene token2 _: %s\n", token);
                //printf("Read token: %s\n", token); 

                //Cuando leo el último ingrediente cambio un \n por un \0
                if(i == numingredients - 1){
                    char *s = token; //Definimos un apuntador a char, y token es otro apuntador
                    //si token no fuera un apuntador tocaba poner &token si fuera una variable
                    while(*s != '\n'){                       
                        ++s;                        
                    }
                    *s = '\0';
                }
                //printf("%s\n", token);

                //cantidad de ingredientes diferentes.
                int comparisonSomeIsEqual = -1; //Si algún ingrediente es igual
                for(int j=0; j<numOfDifferentIngredients; j++){
                    if(strcmp((ingredients+(j*21)), token) == 0){ //Compara si es el mismo ingrediente
                        comparisonSomeIsEqual = 0; //Cambia esto y rompe el ciclo
                        break;
                    }
                }

                if(comparisonSomeIsEqual == -1){
                    strcpy((ingredients+(numOfDifferentIngredients*21)), token);
                    //Va separando de a 21 caracteres                    
                    numOfDifferentIngredients++;
                }
                
            }
        }    
    }
    
    fclose(fileRef);  //Cerramos y le mandamos la dirección del apuntador

    /*
    CONSTRUIMOS LA MATRIZ DE ACUERDO AL NÚMERO DE PLATOS PEDIDOS CON EL NÚMERO
    DE INGREDIENTES Y LA INICIALIZAMOS EN CEROS.
    */
    int **P;
    P = (int**)malloc(sizeof(int*)*numOfDifferentIngredients);
    for(int i=0; i<numOfDifferentIngredients; i++){
        P[i] = (int*)malloc(sizeof(int)*PP);
    }

    for(int i=0; i<numOfDifferentIngredients; i++){
        for(int j=0; j<PP; j++){
            P[i][j] = 0;
        }
    }

    
//-------
    FILE *fileRef2 = fopen(argv[1], "r"); 

    char line2[1024];
    int linecount2 = 0;
    int numingredients2 = 0;

    while(fgets(line2, sizeof(line2), fileRef2)){
        char* word;
        char* rest2 = line2;

        if(linecount2 == 0){
            linecount2++;
        }
        else{
            word = strtok_r(rest2, " ", &rest2);
            numingredients2 = atoi(word);

            //LEYENDO CADA UNO DE LOS INGREDIENTES DEL PLATO
            for(int i=0; i<numingredients2; i++){
                word = strtok_r(rest2, " ", &rest2);
                //printf("---%s", word);               
                //TO BE CONTINUE... :(!
                if(i == numingredients2 - 1){
                    char *s2 = word;
                    while(*s2 != '\n'){
                        ++s2;
                    }
                    *s2 = '\0';
                }

            }
            linecount2++;
        }
    }
    

    

    fclose(fileRef2);
    
    if(NTPP != PP){
        printf("La cantidad de platos ingresada no coincide con lo que se estima. Revisa el archivo! \n");
        return EXIT_FAILURE;
    }

    /*
    printf("PP (Cantidad de platos pedidos) = %d\n", PP);
    printf("P2 (Cantidad de pedidos de dos platos) = %d\n", P2);
    printf("P3 (Cantidad de pedidos de tres platos) = %d\n", P3);
    printf("P2 (Cantidad de pedidos de cuatro platos) = %d\n", P4);
    printf("NTPP (Número total de platos) = %d\n", NTPP);
    printf("I (Cantidad de ingredientes total que no se repiten) = %d\n", numOfDifferentIngredients);    
    printf("Lista de ingredientes que no se repiten: \n");
    */
    for(int i=0; i<numOfDifferentIngredients; i++){
        printf("%s\n", (ingredients + (i*21)) + 0 ); //Array que contiene la lista de ingredientes
    }
    
    //int *h = &P[0][0];
    //printf("%i", *h);

    /*
    printf("Matriz  = ");
    for(int i=0; i<numOfDifferentIngredients; i++){
        printf("\n");
        for(int j=0; j<PP; j++){
            printf("%d", P[i][j]);
        }
    }*/
    printf("\n");   


    return EXIT_SUCCESS;
    
}

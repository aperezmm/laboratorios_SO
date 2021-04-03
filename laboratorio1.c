#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//REVISAR PORQUE *WORDS[] PRODUCE UN WARNING

int aux = 0;
int result = 0;
int vectorSolution[9];

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

//FUNCION PARA MAXIMIZAR PEDIDOS DE DOS PLATOS
int calculateOrderWithTwoPlates(int plates, int ingredients, int **matriz, int *solution){
    int counter = 0;
    int a = 0;
    int b = 0;
    for(int i=0; i<plates; i++){
        for(int j=0; j<ingredients; j++){

            a = matriz[j][solution[(2*i)]];
            //printf("[%d,%d] %d %d\t", j, i, matriz[j][solution[(2*i)]], matriz[j][solution[(2*i+1)]]);
            
            b = matriz[j][solution[(2*i+1)]];

            //printf("a%d b%d\n", a, b);
            if((a == 1) || (b == 1)){
                counter = counter + 1;
            }
        }   
    }
    return counter;    
}

int calculateOrderWithThreePlates(int platesTwo, int plates, int ingredients, int **matriz, int *solution){
    int counter = 0;
    int a = 0;
    int b = 0; 
    int c = 0;
    int baseSpace = 2*platesTwo; 
    for(int i=0; i<plates; i++){
        for(int j=0; j<ingredients; j++){

            a = matriz[j][solution[baseSpace+(3*i)]];
            //printf("[%d,%d] %d %d\t", j, i, matriz[j][solution[(2*i)]], matriz[j][solution[(2*i+1)]]);            
            b = matriz[j][solution[baseSpace+(3*i+1)]];
            c = matriz[j][solution[baseSpace+(3*i+2)]];
            //printf("a%d b%d\n", a, b);
            if((a == 1) || (b == 1) || (c == 1)){
                counter = counter + 1;
            }
        }   
    }
    return counter;    
}

int calculateOrderWithFourPlates(int platesTwo, int platesThree, int plates, int ingredients, int **matriz, int *solution){
    int counter = 0;
    int a = 0;
    int b = 0; 
    int c = 0;
    int d = 0;
    int baseSpace = 2*platesTwo+3*platesThree;
    for(int i=0; i<plates; i++){
        for(int j=0; j<ingredients; j++){

            a = matriz[j][solution[baseSpace+(4*i)]];
            //printf("[%d,%d] %d %d\t", j, i, matriz[j][solution[(2*i)]], matriz[j][solution[(2*i+1)]]);            
            b = matriz[j][solution[baseSpace+(4*i+1)]];
            c = matriz[j][solution[baseSpace+(4*i+2)]];
            d = matriz[j][solution[baseSpace+(4*i+3)]];
            //printf("a%d b%d\n", a, b);
            if((a == 1) || (b == 1) || (c == 1)){
                counter = counter + 1;
            }
        }   
    }
    return counter;    
}
void swap (int *x, int *y)
{
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}


void print(int *solutionVector, int plates, int platesTwo, int platesThree, int platesFour, int ingredients, int **matriz)
{
    int i;
    int size = plates;
    
    if (solutionVector != 0) {
        for ( i = 0; i < size; i++) {
            int a = calculateOrderWithTwoPlates(platesTwo, ingredients, matriz, solutionVector);
            int b = calculateOrderWithThreePlates(platesTwo, platesThree, ingredients, matriz, solutionVector);
            int c = calculateOrderWithFourPlates(platesTwo, platesThree, platesFour, ingredients, matriz, solutionVector);
            result = a + b + c; 
            if(result > aux){
                aux = result; //Retornamos la cantidad maxima falta retornar el vector
                memcpy(vectorSolution, solutionVector, size*sizeof(int));
                
            }
        }  
    }    
}



void heappermute(int *solutionVector, int n, int plates, int platesTwo, int platesThree, int platesFour, int ingredients, int **matriz) {
    int i;
    if (n == 1) {
        print(solutionVector, plates, platesTwo, platesThree, platesFour, ingredients, matriz);
	}  
    else {
        for (i = 0; i < n; i++) {
            heappermute(solutionVector, n-1, plates, platesTwo, platesThree, platesFour, ingredients, matriz);
            if (n % 2 == 1) {
                swap(&solutionVector[0], &solutionVector[n-1]);
	        }
            else {
                swap(&solutionVector[i], &solutionVector[n-1]);
            }
	    }
    }
}


int main(int argc, char*argv[]){

    char *ingredients = (char*)malloc(2100*sizeof(char)); //Para resevar memoria, ingredients va ser un apuntador
    char *archiveName = "output.txt";


    //char* para poder luego hacer aritmetica
    //char ingredients[2100]; pero no estariamos reservando
    if(argc < 2){
        printf("You must specify a filepath\n");
        return EXIT_FAILURE;
    } 
    //Apuntador
    FILE* fileRef = fopen(argv[1], "r");

    if(!fileRef){
        printf("Error opening the file %s\n", argv[1]);
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


    if(NTPP != PP){
        printf("La cantidad de platos ingresada no coincide con lo que se estima. Revisa el archivo! \n");
        return EXIT_FAILURE;
    }
    int solutionVector[NTPP];
    for(int i=0; i<NTPP; i++){
        solutionVector[i] = i;
        //printf("-%d", solutionVector[i]);
    }

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

    FILE *fileRef2 = fopen(argv[1], "r"); 

    char line2[1024];
    int linecount2 = 0;
    int numingredients2 = 0;
    int ingredientAuxIndex = -1;

    //LEEMOS EL ARCHIVO LINEA POR LINEA PARA EMPEZAR A LLENAR LA MATRIZ
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
                if(i == numingredients2 - 1){
                    char *s2 = word;
                    while(*s2 != '\n'){
                        ++s2;
                    }
                    *s2 = '\0';
                }

                //BUSCAMOS EL INGREDIENTE ACTUAL EN EL ARRAY DE INGREDIENTES
                for(int j=0; j < numOfDifferentIngredients; j++){
                    if(strcmp((ingredients+(j*21)), word) == 0){
                        ingredientAuxIndex = j;
                        break;
                    }
                }

                //VALIDAMOS QUE SI ENCUENTRE LA PALABRA
                if(ingredientAuxIndex == -1){
                    printf("La palabra %s no se encontró", word);
                    return EXIT_FAILURE;
                }

                //EMPEZAMOS A LLENAR LA MATRIZ
                P[ingredientAuxIndex][linecount2-1] = 1;
            }
            linecount2++;
        }
    } 

    fclose(fileRef2);

    heappermute(solutionVector, NTPP, NTPP, P2, P3, P4, numOfDifferentIngredients, P);


    char *ingredientsForPlateTwo = (char*)malloc(2100*sizeof(char));
    char *ingredientsForPlateThree = (char*)malloc(2100*sizeof(char));
    char *ingredientsForPlateFour = (char*)malloc(2100*sizeof(char));

    char *ingredientTwo;
    char *ingredientThree;
    char *ingredientFour;

    int numOfDifferentIngredientsForPlateTwo = 0;
    int numOfDifferentIngredientsForPlateThree= 0;
    int numOfDifferentIngredientsForPlateFour = 0;

    int auxPlatesTwo = 0;
    int auxPlatesThree = 0;
    int auxPlatesFour = 0;
    


    for(int j=0; j < NTPP; j++){
        for(int i=0; i < numOfDifferentIngredients; i++){
            
            int comparisonIngredientsExist = - 1;

            if(auxPlatesTwo < (2*numOfDifferentIngredients)){
                if(P[i][vectorSolution[j]] == 1){
                    ingredientTwo = ingredients+(i*21);
                    for(int h=0; h < numOfDifferentIngredients; h++){
                        if(strcmp(ingredientsForPlateTwo+(h*21), ingredientTwo) == 0){
                            comparisonIngredientsExist = 0;
                            break;
                        }
                    }

                    if(comparisonIngredientsExist == -1){
                        strcpy(ingredientsForPlateTwo+(numOfDifferentIngredientsForPlateTwo*21), ingredientTwo);
                        numOfDifferentIngredientsForPlateTwo++;
                    }
                    
                }
                auxPlatesTwo++;
                
            }

            else if(auxPlatesThree < (3*numOfDifferentIngredients)){
                if(P[i][vectorSolution[j]] == 1){
                    ingredientThree = ingredients+(i*21);
                    for(int h=0; h < numOfDifferentIngredients; h++){
                        if(strcmp(ingredientsForPlateThree+(h*21), ingredientThree) == 0){
                            comparisonIngredientsExist = 0;
                            break;
                        }
                    }

                    if(comparisonIngredientsExist == -1){
                        strcpy(ingredientsForPlateThree+(numOfDifferentIngredientsForPlateThree*21), ingredientThree);
                        numOfDifferentIngredientsForPlateThree++;
                    }
                    
                }
                auxPlatesThree++;
                
            }

            else if(auxPlatesFour < (4 * numOfDifferentIngredients)){
                if(P[i][vectorSolution[j]] == 1){
                    ingredientFour = ingredients+(i*21);
                    for(int h=0; h < numOfDifferentIngredients; h++){
                        if(strcmp(ingredientsForPlateFour+(h*21), ingredientFour) == 0){
                            comparisonIngredientsExist = 0;
                            break;
                        }
                    }

                    if(comparisonIngredientsExist == -1){
                        strcpy(ingredientsForPlateFour+(numOfDifferentIngredientsForPlateFour*21), ingredientFour);
                        numOfDifferentIngredientsForPlateFour++;
                    }
                    
                }
                auxPlatesFour++;
                
            }
            
        }
    }

    FILE *archive = fopen(archiveName, "w");

    if(archive == NULL){
        printf("Error abriendo archivo %s", archiveName);
        return EXIT_FAILURE;
    }

    printf("\n");
    printf("Esta es la matriz de los platos: ");
    printf("\n");
    for(int i=0; i<numOfDifferentIngredients; i++){
        printf("\n");
        for(int j=0; j<PP; j++){
            printf("%3d", P[i][j]);
        }
    }
    printf("\n");

    printf("\n");
    printf("Este es el vector solucion: ");
    for(int j = 0; j < NTPP; j++){
        printf("%3d", vectorSolution[j]);
    }
    printf("\n");

    printf("\n");
    printf("La cantidad de ingredientes diferentes totales es: %d", aux);
    printf("\n");
    printf("\n");

    printf("El pedido 0 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateTwo; i++){
        printf("%s, ", (ingredientsForPlateTwo+(i*21))+0);
    }
    printf("\n");

    printf("El pedido 1 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateThree; i++){
        printf("%s, ", (ingredientsForPlateThree+(i*21))+0);
    }
    printf("\n");

    printf("El pedido 2 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateFour; i++){
        printf("%s, ", (ingredientsForPlateFour+(i*21))+0);
    }

    //ESCRIBIMOS EN EL ARCHIVO
    fprintf(archive, "\n");
    fprintf(archive, "Esta es la matriz de los platos: ");
    fprintf(archive,"\n");
    for(int i=0; i<numOfDifferentIngredients; i++){
        fprintf(archive, "\n");
        for(int j=0; j<PP; j++){
            fprintf(archive,"%3d", P[i][j]);
        }
    }
    fprintf(archive,"\n");

    fprintf(archive,"\n");
    fprintf(archive,"Este es el vector solucion: ");
    for(int j = 0; j < NTPP; j++){
        fprintf(archive, "%3d", vectorSolution[j]);
    }
    fprintf(archive,"\n");

    fprintf(archive,"\n");
    fprintf(archive,"La cantidad de ingredientes diferentes totales es: %d", aux);
    fprintf(archive,"\n");
    fprintf(archive,"\n");

    fprintf(archive,"El pedido 0 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateTwo; i++){
        fprintf(archive,"%s, ", (ingredientsForPlateTwo+(i*21))+0);
    }
    fprintf(archive,"\n");

    fprintf(archive,"El pedido 1 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateThree; i++){
        fprintf(archive,"%s, ", (ingredientsForPlateThree+(i*21))+0);
    }
    fprintf(archive,"\n");

    fprintf(archive,"El pedido 2 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateFour; i++){
        fprintf(archive,"%s, ", (ingredientsForPlateFour+(i*21))+0);
    }

    fclose(archive);
    printf("\n");
    printf("\n");
    puts("Contenido escrito correctamente");

    return EXIT_SUCCESS;
    
}

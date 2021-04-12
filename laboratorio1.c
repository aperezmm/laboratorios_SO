#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int aux = 0;
int result = 0;
int vectorSolution[20];

unsigned countWords(char *str){
    int state = 0;
    unsigned wc = 0;  // word count
    while (*str){
        if (*str == ' ' || *str == '\n' || *str == '\t'){
            state = 0;
        }
        else if (state == 0){
            state = 1;
            ++wc;
        }
        
        ++str;
    }
 
    return wc;
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

//FUNCION PARA MAXIMIZAR PEDIDOS DE TRES PLATOS
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

//FUNCION PARA MAXIMIZAR PEDIDOS DE CUATRO PLATOS
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

//ALGORITMO HEAP PARA LAS PERMUTACIONES
void swap (int *x, int *y){
    int temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

//FUNCIÓN PARA CALCULAR CANTIDAD MAXIMA DE INGREDIENTES POR CADA VECTOR SOLUCIÓN
void print(int *solutionVector, int plates, int platesTwo, int platesThree, int platesFour, int ingredients, int **matriz){
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

//FUNCIÓN PARA REALIZAR PERMUTACIÓN
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

//METODO PRINCIPAL DE LA APLICACIÓN
int main(int argc, char*argv[]){

    //RESERVAMOS MEMORIA
    char *ingredients = (char*)malloc(2100*sizeof(char)); 
    char *archiveName = "output.txt";

    //VALIDACIÓN DEL ARCHIVO
    if(argc < 2){
        printf("Usted debe especificar un archivo, por favor vuelva a intentarlo.\n");
        printf("EJEMPLO EN CONSOLA: ./laboratorio file.txt \n");
        return EXIT_FAILURE;
    } 
    //APUNTADOR
    FILE* fileRef = fopen(argv[1], "r");

    //VERIFICAR QUE EL ARCHIVO SE HAYA ABIERTO CORRECTAMENTE
    if(!fileRef){
        printf("Ocurrió un error al abrir el archivo, intente nuevamente. %s\n", argv[1]);
        return EXIT_FAILURE;
    }
    char line[1024]; //Si la linea mide más de esto, solo toma lo que quepa

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

    /*
    SE OBTIENE LA CANTIDAD DE PLATOS PEDIDOS, LA CANTIDAD DE INGREDIENTES
    DIFERENTES, CANTIDAD DE PEDIDOS DE DOS, TRES Y CUATRO PLATOS, EL NÚMERO
    TOTAL DE PLATOS PEDIDOS Y SE OBTIENE LOS INGREDIENTES DIFERENTES.
    */
    while(fgets(line, 1024, fileRef)){
        //printf("Linea: %s\n", line);
        char* token;
        char* rest = line; //CONTIENE LA LINEA LEÍDA
        if(linecount == 0){
            while((token = strtok_r(rest, " ", &rest))){
                quantities[termscount] = atoi(token);
                termscount++;
                
            }
            linecount++;
            PP = quantities[0]; //CANTIDAD DE PLATOS PEDIDOS
            P2 = quantities[1]; //CANTIDAD DE PEDIDOS DE DOS PLATOS
            P3 = quantities[2]; //CANTIDAD DE PEDIDOS DE TRES PLATOS
            P4 = quantities[3]; //CANTIDAD DE PEDIDOS DE CUATRO PLATOS
            NTPP = P2 * 2 + P3 * 3 + P4 * 4; //NUMERO TOTAL DE PLATOS PEDIDOS

            if(((2*P2)+(3*P3)+(4*P4)) != PP){
                printf("La cantida de pedidos es diferente a la inicial, verifique el archivo! \n");
                return EXIT_FAILURE;
            }
        }
        else{
            //APARTIR DE LA SEGUNDA LÍNEA DEL ARCHIVO SE EJECUTA
            token = strtok_r(rest, " ", &rest);
            numingredients = atoi(token);

            //TO DO: Verificar que el numingredients sea realmente el 
            //numero de palabras que tiene la linea
            if( countWords(rest) != numingredients ){
                printf("La cantidad de ingredientes es la linea [%s], no es igual a %d", rest, numingredients);
                return EXIT_FAILURE;
            }

            //LEEMOS CADA UNO DE LOS INGREDIENTES DE LA LINEA
            for(int i=0; i<numingredients; i++){
                token = strtok_r(rest, " ", &rest);

                //FUNCIÓN PARA CAMBIAR EL FIN DE LINEA POR FIN DE STRING
                if(i == numingredients - 1){
                    char *s = token;
                    while(*s != '\n'){                       
                        ++s;                        
                    }
                    *s = '\0';
                }

                int comparisonSomeIsEqual = -1; 
                for(int j=0; j<numOfDifferentIngredients; j++){
                    if(strcmp((ingredients+(j*21)), token) == 0){ //COMPARA SI UN INGREDIENTE EXISTE
                        comparisonSomeIsEqual = 0; //CAMBIA ESTADO DE VARIABLE Y ROMPE CICLO
                        break;
                    }
                }
                //FUNCIÓN PARA GUARDAR EL NUEVO INGREDIENTE Y AUMENTAR LA CANTIDAD DE INGREDIENTES DIFERENTES.
                if(comparisonSomeIsEqual == -1){
                    strcpy((ingredients+(numOfDifferentIngredients*21)), token);                   
                    numOfDifferentIngredients++;
                }                
            }
            linecount++;
        }    
    }
    
    fclose(fileRef);  //FINALIZAMOS LA PRIMER LEÍDA DEL ARCHIVO

    //VALIDACIÓN PARA VERIFICAR QUE EL NÚMERO TOTAL DE PLATOS SEA IGUAL A LA CANTIDAD DE PLATOS PEDIDOS
    if(NTPP != PP){
        printf("La cantidad de platos ingresada no coincide con lo que se estima. Revisa el archivo! \n");
        return EXIT_FAILURE;
    }  
    
    if(PP != (linecount-2)){
        printf("La cantidad de platos pedidos es diferente a la cantidad de platos ingresados, por favor verifique el archivo!\n");
        return EXIT_FAILURE;
    }

    //FUNCIÓN PARA INICIALIZAR VECTOR SOLUCIÓN
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

    //LEEMOS EL ARCHIVO NUEVAMENTE
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

    /*HACEMOS LLAMADO AL METODO DE HIPERMUTAR, RECIBE TODOS ESOS PARAMETROS PARA REALIZAR LA RESPECTIVA
    VALIDACIÓN POR CADA RESULTADO DE LA PERMUTACIÓN*/
    heappermute(solutionVector, NTPP, NTPP, P2, P3, P4, numOfDifferentIngredients, P);

    //RESERVAMOS ESPACIO DE MEMORIA
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
    

    /*
    FUNCIÓN PARA RECORRER LA MATRIZ SEGÚN LA POSICIÓN DEL MEJOR VECTOR SOLUCIÓN, Y TOMAR
    CADA UNO DE LOS INGREDIENTES Y GUARDARLO.
    */
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

    FILE *archive = fopen(archiveName, "w"); //APUNTADOR AL ARCHIVO EN EL QUE VAMOS A ESCRIBIR

    if(archive == NULL){
        printf("Error abriendo archivo %s", archiveName);
        return EXIT_FAILURE;
    }

    //PRINTFS PARA LA CONSOLA
    int auxthree = 0;

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
    

    printf("El pedido 0 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateTwo; i++){
        printf("%s, ", (ingredientsForPlateTwo+(i*21))+0);
        auxthree++;
    }
    printf("\n");

    printf("El pedido 1 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateThree; i++){
        printf("%s, ", (ingredientsForPlateThree+(i*21))+0);
        auxthree++;
    }
    printf("\n");

    printf("El pedido 2 contiene: ");
    for(int i=0; i<numOfDifferentIngredientsForPlateFour; i++){
        printf("%s, ", (ingredientsForPlateFour+(i*21))+0);
        auxthree++;
    }
    printf("\n");

    printf("\n");
    printf("La cantidad de ingredientes diferentes totales es: %d", auxthree);
    printf("\n");

    //REALIZAMOS LA ESCRITURA EN EL ARCHIVO    
    fprintf(archive, "Realizado por: Alejandro Pérez & Juan Felipe Santa!");
    fprintf(archive, "\n");
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
    fprintf(archive,"\n");
    fprintf(archive,"\n");
    fprintf(archive,"La cantidad de ingredientes diferentes totales es: %d", auxthree);
    fprintf(archive,"\n");

    fclose(archive);
    printf("\n");
    printf("\n");
    puts("Contenido escrito correctamente");
    printf("\n");

    return EXIT_SUCCESS;
    
}

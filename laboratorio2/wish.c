#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_SIZE 100


char* path_directory[] = {
    "/bin/",
    "/home/alejandro/binarios/",
    NULL
};

char my_path[MAX_SIZE];

typedef enum{custom_exit, custom_cd, custom_path, not_found} builtin_command;

const static struct{
    builtin_command command;
    char* string_command;
} commands[] = {
    {custom_exit, "exit"},
    {custom_cd, "cd"},
    {custom_path, "path"}
};

struct SplittedResponse{
    int size;
    char* data;
};

builtin_command str_to_command(char* strcommand){
    int builtin_command_quantity = 3;
    for(int i = 0; i < builtin_command_quantity; i++){
        if(!strcmp(strcommand, commands[i].string_command)){
            return commands[i].command;
        }
    }
    return not_found;
}

void handle_error(char* c){
    printf("An error has occurred\n");
}

int actual_directory(){
    printf("%s\n", getcwd(my_path, MAX_SIZE));
}

int change_directory(struct SplittedResponse splitted_command){
    //TO DO: PREGUNTAR
    int child = fork();
    printf("%d\n", child);

    if(child < 0){
        handle_error("a");
    }
    else if(child == 0)
    {   
        if (splitted_command.size >= 3){
            printf("wish: too many arguments\n");
            return -1;
        }

        if (splitted_command.size == 1){
            chdir(getenv("HOME"));
            actual_directory();
            return 0;
        }                  

        char* route = splitted_command.data+(21);
        int result = chdir(route);

        actual_directory();
        printf("Después de ejecutar el proceso hijo\n");

        if(result == -1){
            printf("wish: No such file or directory\n");
            return -1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        int wait_child = wait(NULL);
        printf("Después de esperar al hijo\n");
    }

    
}

struct SplittedResponse split_command_argument(char *command, char *delimiter){
    printf("split_command_argument command --%s delimiter --%s\n", command, delimiter);
    
    char *data_splitted = malloc(2100*sizeof(char*));
    int numOfArguments = 0;
    
    char *complete_result = strdup(command);

    char *tok = complete_result, *end = complete_result;
    while(tok != NULL){
        strsep(&end, delimiter);
        printf("%s\n", tok);
        
        char *s = tok;
        if(*s != '\0'){
            strcpy((data_splitted+(numOfArguments*21)),tok);
            numOfArguments++;
        }        
        tok = end;      
        
               
    }

    struct SplittedResponse response;
    response.size = numOfArguments;
    response.data = data_splitted;
    return response;
}

int run_command_in_path(struct SplittedResponse splitted_command){
    int child = fork();
    printf("%d\n", child);

    if(child < 0){
        handle_error("a");
    }
    else if(child == 0)
    {   
        
        int i = 0;
        char pathToFile[MAX_SIZE];
        
        int file_exists = -1;
        
        while(*(path_directory + i) != NULL && file_exists == -1){
            strcpy(pathToFile, *(path_directory + i));
            strcat(pathToFile, splitted_command.data);
            file_exists = access(pathToFile, X_OK);
            i++;
        }
        printf("pathtofile: %s\n", pathToFile);
        printf("file_exists: %d\n", file_exists);

        if(file_exists != -1){
            char *args[3];
            args[0] = strdup(pathToFile);
            args[1] = strdup("/home");
            args[2] = NULL;
            execvp(args[0], args);
        }

        if(file_exists == -1){
            printf("Comando no encontrado\n");
        }
    }
    else
    {
        int wait_child = wait(NULL);
        printf("Después de esperar al hijo\n");
    }

}



int main(int argc, char* argv[]){
    char str[MAX_SIZE];

    do
    {
        printf("wish> ");
        fgets(str, MAX_SIZE, stdin);
        struct SplittedResponse splitted_command;

        
        char* p = str;
        while(*p != '\n'){
            p++;
        }
        *p = '\0';
        
        splitted_command = split_command_argument(str, " ");

        //IMPRIMIMOS EL ARRAY DE ELEMENTOS SEPARADOS POR EL CARACTER
        for (int i = 0; i < splitted_command.size; i++){
            printf("SPLITTED %d, %s\n", i, splitted_command.data+(i*21));
        }
        builtin_command command = str_to_command(splitted_command.data);
        
        if(command != not_found){
            switch(command)
            {
                case custom_exit:
                    exit(0);
                    break;
                case custom_cd:
                    change_directory(splitted_command);                     

                    break;
                case custom_path:
                    printf("Cambiar el path_directory\n");
                    break;
                default:
                    printf("#######################\n");
            }
        }
        else
        {
            printf("Buscar en el path\n");

            run_command_in_path(splitted_command);
        }

    }while(1);
} 
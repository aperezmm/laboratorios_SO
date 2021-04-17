#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_SIZE 100


char* path_directory[] = {
    "/bin/",
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

builtin_command str_to_command(char* strcommand){
    int builtin_command_quantity = 3;
    for(int i = 0; i < builtin_command_quantity; i++){
        if(!strcmp(strcommand, commands[i].string_command)){
            return commands[i].command;
        }
    }
    return not_found;
}

int actual_directory(){
    printf("%s\n", getcwd(my_path, MAX_SIZE));
}

int change_directory(char strdirectory[]){

}



int main(int argc, char* argv[]){
    char str[MAX_SIZE];

    do
    {
        printf("wish> ");
        fgets(str, MAX_SIZE, stdin);
        char* p = str;

        while(*p != '\n'){
            p++;
        }
        *p = '\0';

        builtin_command command = str_to_command(str);
        printf("%d %s\n", command, str);
        //Si es uno de los comandos 
        if(command != not_found){
            switch(command)
            {
                case custom_exit:
                    exit(0);
                    break;
                case custom_cd:
                    actual_directory(); //Directorio actual
                    //Separar y pasar el argumento
                    chdir("..");
                    actual_directory();
                    chdir("laboratorio2");
                    actual_directory();
                    printf("Cambiar de directorio\n");
                    
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
        }

    }while(1);
} 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_SIZE 100

char *path_directory;

char my_path[MAX_SIZE];

typedef enum
{
    custom_exit,
    custom_cd,
    custom_path,
    not_found
} builtin_command;

const static struct
{
    builtin_command command;
    char *string_command;
} commands[] = {
    {custom_exit, "exit"},
    {custom_cd, "cd"},
    {custom_path, "path"}};

struct SplittedResponse
{
    int size;
    char *data;
};

builtin_command str_to_command(char *strcommand)
{
    int builtin_command_quantity = 3;
    for (int i = 0; i < builtin_command_quantity; i++)
    {
        if (!strcmp(strcommand, commands[i].string_command))
        {
            return commands[i].command;
        }
    }
    return not_found;
}

void handle_error(char *c)
{
    printf("An error has occurred\n");
}

int actual_directory()
{
    printf("%s\n", getcwd(my_path, MAX_SIZE));
}

int change_directory(struct SplittedResponse splitted_command)
{
    if (splitted_command.size >= 3)
    {
        //printf("wish: too many arguments\n");
        return -1;
    }

    if (splitted_command.size == 1)
    {
        chdir(getenv("HOME"));
        actual_directory();
        return 0;
    }

    char *route = splitted_command.data + (21);
    int result = chdir(route);

    actual_directory();
    //printf("Después de ejecutar el proceso hijo\n");

    if (result == -1)
    {
        //printf("wish: No such file or directory\n");
        return -1;
    }
    else
    {
        return 0;
    }
}

struct SplittedResponse split_command_argument(char *command, char *delimiter)
{
    //printf("split_command_argument command --[%s] delimiter --[%s]\n", command, delimiter);

    char *data_splitted = malloc(2100 * sizeof(char *));
    int numOfArguments = 0;

    char *complete_result = strdup(command);

    char *tok = complete_result, *end = complete_result;
    while (tok != NULL)
    {
        strsep(&end, delimiter);

        //printf("%s\n", tok);
        //ls /home > output.txt
        char *s = tok;
        if (*s != '\0')
        {
            strcpy((data_splitted + (numOfArguments * 21)), tok);
            numOfArguments++;
        }

        tok = end;
    }

    struct SplittedResponse response;
    response.size = numOfArguments;
    response.data = data_splitted;
    return response;
}

int is_command_with_redirection(struct SplittedResponse splitted_command)
{
    int redirection_symbol_count = 0;
    //RECORRER CADA COMANDO Y CUENTA LA CANTIDAD DE '>'
    for (int i = 0; i < splitted_command.size; i++)
    {
        char *command_arg = splitted_command.data + (i * 21);
        while (*command_arg != '\0')
        {
            if (*command_arg == '>')
            {
                redirection_symbol_count++;
            }
            command_arg++;
        }
    }
    //printf("redirection_symbol_count [%d]\n", redirection_symbol_count);

    //VALIDAMOS QUE EXISTA SOLO UN > Y ESTE EN LA POSICIÓN PENULTIMA
    if (redirection_symbol_count == 1)
    {
        int index = splitted_command.size - 2;
        //printf("index_data [%s]\n", splitted_command.data+(index*21));
        int is_equal = strcmp(splitted_command.data + (index * 21), ">");
        if (is_equal == 0)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else if (redirection_symbol_count == 0)
    {
        return 0;
    }
    else
    {
        return -1; //ERROR
    }
}

int run_command_with_params(char *binary_path, struct SplittedResponse splitted_command)
{
    //TO DO:
    /*
    char* args = malloc(2100*sizeof(char*));
    strcpy(args, strdup(binary_path));
    for (int i = 1; i < splitted_command.size; i++){
        printf("ARG[%d] = [%s]\n", i, splitted_command.data+(i*21));
        strcpy(args+(i*21), strdup(splitted_command.data+(i*21)));
    }

    //strcpy(args+(splitted_command.size*21), NULL);

    /*
    for (int i = 0; i < splitted_command.size; i++){
        printf("ARG ???????????????[%d] = [%s]\n", i, args[i]);
    }
    return args;*/

    //ls /home>output.txt

    int is_redirection = is_command_with_redirection(splitted_command);
    printf("is_redirection: %d\n", is_redirection);

    // redirection with error.
    if (is_redirection == -1)
    {
        handle_error("Redirección con error");
    }

    if (is_redirection == 1)
    {
        close(STDOUT_FILENO);
        int output_index = splitted_command.size - 1;
        char *output_name = splitted_command.data + (output_index * 21);
        open(output_name, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        char *args[splitted_command.size - 2];

        for (int i = 0; i < splitted_command.size - 2; i++)
        {
            //printf("ARG[%d] = [%s]\n", i, splitted_command.data+(i*21));
            args[i] = strdup(splitted_command.data + (i * 21));
        }
        args[splitted_command.size - 2] = NULL;
        int response = execvp(binary_path, args);
    }
    else
    {
        char *args[splitted_command.size + 1];
        args[0] = strdup(binary_path);
        for (int i = 1; i < splitted_command.size; i++)
        {
            //printf("ARG[%d] = [%s]\n", i, splitted_command.data+(i*21));
            args[i] = strdup(splitted_command.data + (i * 21));
        }
        args[splitted_command.size] = NULL;
        int response = execvp(binary_path, args);
        printf("RESPONSE RUN_COMMAND_WITH_PARAMS %d\n", response);
    }
}

int update_path(struct SplittedResponse splitted_command)
{
    if (splitted_command.size == 1)
    {
        //limpiar la variable path completamente
        strcpy(path_directory, "NULL");
    }
    else
    {

        for (int i = 1; i < splitted_command.size; i++)
        {

            char *p = splitted_command.data + (i * 21);

            while (*p != '\0')
            {
                p++;
            }
            p--;
            if (*p != '/')
            {
                p++;
                *p = '/';
            }
            else
            {
                p++;
                *p = '\0';
            }

            strcpy(path_directory + (i - 1) * (40), splitted_command.data + (i * 21));
        }
        strcpy(path_directory + ((splitted_command.size - 1) * 40), "NULL");
    }

    /*
    for(int i=0; i< splitted_command.size; i++){
        printf("PATH DIRECTORY %s\n", path_directory+(i*40));
    }*/
}

int run_command_in_path(struct SplittedResponse splitted_command)
{
    int child = fork();
    // printf("%d\n", child);

    if (child < 0)
    {
        handle_error("a");
    }
    else if (child == 0)
    {
        int i = 0;
        char pathToFile[MAX_SIZE];

        int is_executable = -1;

        while ((path_directory + (i * 40)) != "NULL" && is_executable == -1)
        {
            strcpy(pathToFile, (path_directory + (i * 40)));
            strcat(pathToFile, splitted_command.data);
            //TO DO: Verificar que el archivo exista
            is_executable = access(pathToFile, X_OK);
            i++;
        }
        // printf("pathtofile: %s\n", pathToFile);

        if (is_executable != -1)
        {
            return run_command_with_params(pathToFile, splitted_command);
        }
        printf("is executable%d\n", is_executable);
        if (is_executable == -1)
        {
            printf("Comando no encontrado\n");
            return -1;
        }
    }
    else
    {
        int wait_child = wait(NULL);
    }
}

int main(int argc, char *argv[])
{
    char str[MAX_SIZE];

    path_directory = malloc(4000 * sizeof(char *));
    strcpy((path_directory), "/bin/");
    strcpy((path_directory + (40)), "NULL");
    if (argc > 2)
    {
        exit(1);
    }

    if (argc == 2)
    {
        //TO DO: Iniciar el modo batch
    }

    do
    {
        printf("wish> ");
        fgets(str, MAX_SIZE, stdin);
        struct SplittedResponse splitted_command;

        char *p = str;
        while (*p != '\n')
        {
            p++;
        }
        *p = '\0';

        splitted_command = split_command_argument(str, " ");

        //IMPRIMIMOS EL ARRAY DE ELEMENTOS SEPARADOS POR EL CARACTER
        /*
        for (int i = 0; i < splitted_command.size; i++){
            printf("SPLITTED %d, %s\n", i, splitted_command.data+(i*21));
        }*/
        builtin_command command = str_to_command(splitted_command.data);

        if (command != not_found)
        {
            switch (command)
            {
            case custom_exit:
                exit(0);
                break;
            case custom_cd:
                change_directory(splitted_command);

                break;
            case custom_path:
                update_path(splitted_command);

                break;
            default:
                printf("#######################\n");
            }
        }
        else
        {
            printf("Buscando en el path...\n");

            int response = run_command_in_path(splitted_command);
            //printf("response in else %d\n", response);
        }

    } while (1);
}
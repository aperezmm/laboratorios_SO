#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_SIZE 1024
#define MAX_SIZE_COMMAND 1024

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
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

int is_empty_line(char *line)
{
    char *ch;
    int is_empty_line = 1;

    // Iterate through each character.
    for (ch = line; *ch != '\0'; ++ch)
    {
        if (!isspace(*ch))
        {
            // Found a non-whitespace character.
            is_empty_line = 0;
            break;
        }
    }

    return is_empty_line;
}

int change_directory(struct SplittedResponse splitted_command)
{
    if (splitted_command.size == 2)
    {
        char *route = splitted_command.data + (50);
        int result = chdir(route);

        if (result == -1)
        {
            handle_error("No such file or directory");
        }
        else
        {
            return 0;
        }
    }
    else
    {
        handle_error("Bad cd");
    }
}

void str_replace(char *target, const char *needle, const char *replacement)
{
    char buffer[1024] = {0};
    char *insert_point = &buffer[0];
    const char *tmp = target;
    size_t needle_len = strlen(needle);
    size_t repl_len = strlen(replacement);

    while (1)
    {
        const char *p = strstr(tmp, needle);

        // walked past last occurrence of needle; copy remaining part
        if (p == NULL)
        {
            strcpy(insert_point, tmp);
            break;
        }

        // copy part before needle
        memcpy(insert_point, tmp, p - tmp);
        insert_point += p - tmp;

        // copy replacement string
        memcpy(insert_point, replacement, repl_len);
        insert_point += repl_len;

        // adjust pointers, move on
        tmp = p + needle_len;
    }

    // write altered string back to target
    strcpy(target, buffer);
}

struct SplittedResponse split_command_argument(char *command, char *delimiter, int show)
{
    char *data_splitted = malloc(5000 * sizeof(char *));
    int numOfArguments = 0;

    char *complete_result = strdup(command);

    char *tok = complete_result, *end = complete_result;
    while (tok != NULL)
    {
        strsep(&end, delimiter);

        char *s = tok;
        if (*s != '\0')
        {
            strcpy((data_splitted + (numOfArguments * 50)), tok);
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
    //RECORRER CADA PALABRA DEL COMANDO Y CUENTA LA CANTIDAD DE '>'
    for (int i = 0; i < splitted_command.size; i++)
    {
        char *command_arg = splitted_command.data + (i * 50);
        while (*command_arg != '\0')
        {
            if (*command_arg == '>')
            {
                redirection_symbol_count++;
            }
            command_arg++;
        }
    }

    //VALIDAMOS QUE EXISTA SOLO UN > Y ESTE EN LA POSICIÓN PENULTIMA
    if (redirection_symbol_count == 1)
    {
        int index = splitted_command.size - 2;
        int is_equal = strcmp(splitted_command.data + (index * 50), ">");
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
    int is_redirection = is_command_with_redirection(splitted_command);

    // redirection with error.
    if (is_redirection == -1)
    {
        return -1;
    }

    // TODO: verify if it's posible to do it easier.
    if (is_redirection == 1)
    {
        close(STDOUT_FILENO);
        int output_index = splitted_command.size - 1;
        char *output_name = splitted_command.data + (output_index * 50);
        open(output_name, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

        char *args[splitted_command.size - 2];

        for (int i = 0; i < splitted_command.size - 2; i++)
        {
            args[i] = strdup(splitted_command.data + (i * 50));
        }
        args[splitted_command.size - 2] = NULL;
        int resp = execv(binary_path, args);
        return resp;
    }
    else
    {
        char *args[splitted_command.size + 1];
        args[0] = strdup(splitted_command.data);

        for (int i = 1; i < splitted_command.size; i++)
        {
            args[i] = strdup(splitted_command.data + (i * 50));
        }
        args[splitted_command.size] = NULL;
        return execv(binary_path, args);
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

            char *p = splitted_command.data + (i * 50);

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

            strcpy(path_directory + (i - 1) * (40), splitted_command.data + (i * 50));
        }
        strcpy(path_directory + ((splitted_command.size - 1) * 40), "NULL");
    }
}

// Run a single command
int run_command_in_path(struct SplittedResponse splitted_command)
{
    int i = 0;
    char pathToFile[MAX_SIZE];

    int is_executable = -1;
    int response = -1;

    // Search binary in all available directories.
    while (strcmp(path_directory + (i * 40), "NULL") != 0 && is_executable == -1)
    {
        strcpy(pathToFile, (path_directory + (i * 40)));
        strcat(pathToFile, splitted_command.data);
        //TO DO: Verificar que el archivo exista
        is_executable = access(pathToFile, X_OK);
        i++;
    }

    if (is_executable != -1)
    {
        response = run_command_with_params(pathToFile, splitted_command);
    }

    return response;
}

int execute_generic_command(char *generic_command)
{
    struct SplittedResponse parallel_commands;
    char *p = generic_command;

    if (is_empty_line(p) == 1)
    {
        return 0;
    }
    // Validate and empty line

    // Removes the \n and replaced by \0
    while (*p != '\n')
    {
        p++;
    }
    *p = '\0';

    // Normalize the parameters to avoid problems
    str_replace(generic_command, ">", " > ");

    // Verificar si es un parallel command
    parallel_commands = split_command_argument(generic_command, "&", 1);
    int status;
    int pids[parallel_commands.size];
    for (int i = 0; i < parallel_commands.size; i++)
    {
        pids[i] = 0;
    }

    // esto es provicional....
    int response = -365;

    for (int i = 0; i < parallel_commands.size; i++)
    {
        int pid = fork();

        struct SplittedResponse single_command;
        single_command = split_command_argument(parallel_commands.data + (i * 50), " ", 0);
        builtin_command command = str_to_command(single_command.data);

        if (pid < 0)
        {
            printf("Error lauching the process parellel\n");
        }
        else if (pid == 0)
        {
            // CHILD PROCESS USED FOR NOT BUILT-IN COMMANDS ONLY.
            if (command == not_found)
            {
                response = run_command_in_path(single_command);
                if (response == -1)
                {
                    handle_error("run_command_in_path error");
                }
            }
            exit(0);
        }
        else
        {
            // FATHER PROCESS
            if (command != not_found)
            {
                switch (command)
                {
                case custom_exit:
                    if (single_command.size == 1)
                    {
                        exit(0);
                    }
                    else
                    {
                        handle_error("exit bad");
                    }
                    break;

                case custom_cd:
                    change_directory(single_command);
                    break;

                case custom_path:
                    update_path(single_command);
                    break;

                default:
                    printf("####### COMMAND UNHANDLED ########\n");
                }
            }

            pids[i] = pid;
        }
    }

    for (int i = 0; i < parallel_commands.size; i++)
    {
        waitpid(pids[i], &status, 0);
    }
}

int execute_batch_mode(FILE *file)
{
    char line[MAX_SIZE_COMMAND];
    while (fgets(line, 1024, file))
    {
        char *token = line;
        execute_generic_command(line);
    }
    exit(0);
}

int main(int argc, char *argv[])
{
    char str[MAX_SIZE];

    // initialize the pathDirectory variable
    path_directory = malloc(4000 * sizeof(char *));
    strcpy((path_directory), "/bin/");
    strcpy((path_directory + (40)), "NULL");

    struct SplittedResponse splitted_command;

    if (argc > 2)
    {
        handle_error("Many input files");
        exit(1);
    }
    if (argc == 2)
    {

        FILE *fileRef = fopen(argv[1], "r");

        //VERIFICAR QUE EL ARCHIVO SE HAYA ABIERTO CORRECTAMENTE
        if (!fileRef)
        {
            handle_error("Error with file");
            exit(1);
        }

        // TODO verificar los saltos de linea de los archivos. y los Segmentation fault
        int batch_result = execute_batch_mode(fileRef);

        fclose(fileRef);
    }

    // Interactive Mode
    do
    {
        printf("wish> ");
        fgets(str, MAX_SIZE, stdin);
        execute_generic_command(str);
    } while (1);
}
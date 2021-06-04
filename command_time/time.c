#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/time.h>

struct SplittedResponse
{
    int size;
    char *data;
};

void handle_error(char *c)
{
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
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

int run_command_with_params(struct SplittedResponse splitted_command)
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
        int resp = execvp(args[0], args);
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

        return execvp(args[0], args);
    }
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

int execute_generic_command(char *generic_command)
{
    struct SplittedResponse parallel_commands;
    char *p = generic_command;

    // Validate and empty line
    if (is_empty_line(p) == 1)
    {
        return 0;
    }

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

        if (pid < 0)
        {
            printf("Error lauching the process parellel\n");
        }
        else if (pid == 0)
        {
            response = run_command_with_params(single_command);
            if (response == -1)
            {
                handle_error("run_command_in_path error");
            }
            exit(0);
        }
        else
        {
            pids[i] = pid;
        }
    }

    for (int i = 0; i < parallel_commands.size; i++)
    {
        waitpid(pids[i], &status, 0);
    }
}

struct timeval millisDiff(struct timeval start, struct timeval end)
{
    struct timeval result;
    int ONE_M = 1000000;
    int startUs = (start.tv_sec * ONE_M) + start.tv_usec;
    int endUs = (end.tv_sec * ONE_M) + end.tv_usec;

    int diff = endUs - startUs;
    int diff_sec = (int)diff / ONE_M;

    result.tv_sec = diff_sec;
    result.tv_usec = diff - (diff_sec * ONE_M);

    return result;
}

int main(int argc, char *argv[])
{
    // control errors
    if (argc == 1)
    {
        printf("command is required\n");
        exit(1);
    }

    struct timeval start_time;
    struct timeval end_time;

    // execute command
    int pid = fork();
    if (pid < 0)
    {
        printf("Error lauching the process parellel\n");
    }

    gettimeofday(&start_time, NULL);

    if (pid == 0)
    {
        char command_string[500] = "";
        for (int i = 1; i < argc; i++)
        {
            strcat(command_string, argv[i]);
            strcat(command_string, " ");
        }

        return execute_generic_command(command_string);
    }
    else
    {
        // father
        wait(NULL);
        gettimeofday(&end_time, NULL);
        struct timeval diff = millisDiff(start_time, end_time);

        printf("DURATION %d S, %d Us\n", diff.tv_sec, diff.tv_usec);
    }

    return 0;
}
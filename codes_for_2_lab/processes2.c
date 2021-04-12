#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int arg, char *argv[])
{
    printf("Hello world (pid:%d)\n", (int) getpid());
    int rc = fork();
    if(rc < 0)
    {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    }else if(rc == 0)
    {
        printf("Hello, I am a child  (pid:%d)\n", (int) getpid());
    } else //Este else se ejecuta en el padre
    {
        int rc_wait = wait(NULL); //Espera a que termine el primer hijo del proceso padre,  cuando termina el proceso ahora si sigue y termina el proceso
        //Le dije al padre que no siquiera hasta que terminará la ejecución de sus hijos.
        printf("Hello, I am parent of %d (pid:%d)\n", rc, (int) getpid());
    }

    return 0;
}
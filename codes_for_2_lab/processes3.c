#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    printf("Hello world (pid:%d)\n", (int) getpid());
    int rc = fork();

    if(rc < 0){
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if(rc  == 0)
    {   
        /*
            Aqui ya hay algo más elaborado, e invocamos otra función.
        */
        printf("Hello, I am a child  (pid: %d)\n", (int) getpid());
        char *myargs[3];
        myargs[0] = strdup("wc"); //Comando que voy a ejecutar, nombre del programa a ejecutar
        myargs[1] = strdup("Processes3.c"); //Archivo al que le quiero operar
        myargs[2] = NULL; //Por convencion el último argumento en NULO
        //Recibe el nuevo programa que se va a ejecutar, es decir el comando que voy a ejecutar en el hijo. Y como segundo argumento recibe los argumentos que les voy a pasar.
        execvp(myargs[0], myargs); // Nos permite ejecutar cosas diferentes a las que ejecuta el padre.
        printf("This should't print out");
    } else
    {
        int rc_wait = wait(NULL);
        printf("Hello, I am a parent of %d (pid:%d)\n", rc, (int) getpid());
    }
    return 0;
}
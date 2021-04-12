#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int arg, char *argv[]){
    int rc = fork();
    if(rc < 0)
    {
        fprintf(stderr, "Fork failed\n");
        exit(1);
    } else if(rc == 0)
    {
        close(STDOUT_FILENO); //Cierro la salida estandar
        open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU); //Y abro un archivo, otra salida (Los otros son parametros, si no existe lo crea, etc)
        char *myargs[3]; //Apuntador a apuntadores

        //El primer apuntador apunta a
        myargs[0] = strdup("wc"); //Coger el string wc y ponerlo en esta posición de memoria

        //El segundo apuntador apunta a
        myargs[1] = strdup("Processes3.c");

        //El tercer apuntador apunta a null
        myargs[2] = NULL; 
        execvp(myargs[0], myargs); //Nunca retorna, solamente retorna cuando hay un error

        //En el archivo de salida, queda la salida del comando wc
        printf("This shouldn't print out"); //Si se imprime es porque hay un error. Puede ser una forma de validar 
    }else
    {
        int rc_wait = wait(NULL);
    }

    return 0;
}
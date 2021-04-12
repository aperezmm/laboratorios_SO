#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int arg, char *argv[])
{
    printf("Hello world (pid: %d)\n", (int) getpid()); //getpid = identificador del procesos, processId, obtiene el Id del proceso que se ejecuto
    int rc = fork(); /*Crea otro proceso, que es hijo del proceso que se esta ejecutando en el momento y lanza un proceso hijo
        el proceso hijo se crea con una copia exacta del programa que lo lanzo, apartir de la linea donde este lanzada la función fork
        es decir todo lo que este de aquí para abajo se ejecuta.

        Pero tanto el padre y el hijo dentran un valor diferentes
        
    */
    printf("RC %d\n",rc); //Funcionamiento de la función fork, y por eso muestra RC con su valor, revisar la documentación. Para diferenciar cuando ejecuta el padre y cuando el hijo
    //El codigo esta hecho para que se adapte a la variable RC
    if(rc < 0)
    {
        fprintf(stderr, "Error failed!\n");
        exit(1);
    } else if(rc == 0)
    {
        printf("Hello, I am a child (pid: %d)\n", (int) getpid()); //Identificador de procesos diferentes
    } else 
    {
        printf("Hello, I am parent of %d (pid: %d)\n", rc ,(int) getpid()); //Identificador de procesos diferentes
    }

    return EXIT_SUCCESS;
}
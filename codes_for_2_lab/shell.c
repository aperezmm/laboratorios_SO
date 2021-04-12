#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100 //Variable

char* system_path_commands[] = {
    "/bin/"
};

//Lista de comandos que voy a entender (typedef tipo de dato)
typedef enum{jason, abril, diana, not_command, endup} builtin_command;

const static struct{ //Definimos una estructura
    builtin_command command; //Tiene las posibles comandos
    char* string_command;
} commands[] = { //Variable de tipo arriba
    {jason, "jason"}, //Valores segun como esta definido la estructura
    {abril, "abril"},
    {diana, "diana"},
    {endup, "exit"}
};

//Función para através del string obtener el comando
builtin_command str_to_command(char* strcommand){
    //Deberíamos recorrer los comandos que tenemos definidos
    for(int i = 0; i < 4; i++){
        //necesitamos acceder a la variable commands
        //comparo lo que ingreso el usuario con la lista de comandos retorno el correspondiente comando
        if(!strcmp(strcommand, commands[i].string_command)){
            return commands[i].command; 
        }
    }
    //Si no lo encontró
    return not_command;
}

int main(int argc, char*argv[])
{
    char str[MAX_SIZE]; //Solo soportar maximo de 100 caracteres.
    do
    {
        //PROM de la shell indica la ruta donde estoy parado.
        printf("shell >>> ");

        //pedimos al usuario cosas
        //Mandamos el apuntador donde quiero que me guarde lo que me va a ingresar el usuario, el tamaño maximo de lo que va a ingresar al usuario, y de donde va a sacar la información
        fgets(str, MAX_SIZE, stdin);
        char* p = str;
        //Definimos apuntador que apunta a la misma parte.
        while(*p != '\n'){
            p++;
        }
        *p = '\0';

        //invocar y mandamos el comando que terminamos de corregir
        builtin_command command = str_to_command(str); // se supone que en command yo ya tengo el comando
        if(command != not_command){        
            switch(command)
            {
                case jason:
                //En lugar de print definir la funcion y hacer como un execute de la función correspondiente al comando.
                    printf("jason executed\n");
                    break;
                case abril:
                    printf("abril executed\n");
                    break;
                case diana:
                    printf("diana executed\n");
                    break;
                case endup:
                    exit(0); //Esto por que ya esta definido
                default:
                    printf("Command not found\n");
            }
          //Si no es uno de los comandos que tengo predefinidos  
        }else{
            //Ultimo ejemplo de callFork, para que el hijo ejecute una cosa totalmetne diferente al padre
            //REVISAS TAMBIÉN ACCESS = Con este permite identificar si hay un archivo en una ruta. 
            printf("Buscar el comando en la ruta path\n");
        }
    }while(1); //comparamos lo que acabe de leer.
}
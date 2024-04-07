//P2-SSOO-23/24

//  MSH main file
// Write your msh source code here

//#include "parser.h"
#include <stddef.h>			/* NULL */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_COMMANDS 8

int mycalc(char *argv[]);

// files in case of redirection
char filev[3][64];

//to store the execvp second parameter
char *argv_execvp[8];

void siginthandler(int param)
{
	printf("****  Exiting MSH **** \n");
	//signal(SIGINT, siginthandler);
	exit(0);
}


struct command
{
  // Store the number of commands in argvv
  int num_commands;
  // Store the number of arguments of each command
  int *args;
  // Store the commands
  char ***argvv;
  // Store the I/O redirection
  char filev[3][64];
  // Store if the command is executed in background or foreground
  int in_background;
};

int history_size = 20;
struct command * history;
int head = 0;
int tail = 0;
int n_elem = 0;

void free_command(struct command *cmd)
{
    if((*cmd).argvv != NULL)
    {
        char **argv;
        for (; (*cmd).argvv && *(*cmd).argvv; (*cmd).argvv++)
        {
            for (argv = *(*cmd).argvv; argv && *argv; argv++)
            {
                if(*argv){
                    free(*argv);
                    *argv = NULL;
                }
            }
        }
    }
    free((*cmd).args);
}

void store_command(char ***argvv, char filev[3][64], int in_background, struct command* cmd)
{
    int num_commands = 0;
    while(argvv[num_commands] != NULL){
        num_commands++;
    }

    for(int f=0;f < 3; f++)
    {
        if(strcmp(filev[f], "0") != 0)
        {
            strcpy((*cmd).filev[f], filev[f]);
        }
        else{
            strcpy((*cmd).filev[f], "0");
        }
    }

    (*cmd).in_background = in_background;
    (*cmd).num_commands = num_commands-1;
    (*cmd).argvv = (char ***) calloc((num_commands) ,sizeof(char **));
    (*cmd).args = (int*) calloc(num_commands , sizeof(int));

    for( int i = 0; i < num_commands; i++)
    {
        int args= 0;
        while( argvv[i][args] != NULL ){
            args++;
        }
        (*cmd).args[i] = args;
        (*cmd).argvv[i] = (char **) calloc((args+1) ,sizeof(char *));
        int j;
        for (j=0; j<args; j++)
        {
            (*cmd).argvv[i][j] = (char *)calloc(strlen(argvv[i][j]),sizeof(char));
            strcpy((*cmd).argvv[i][j], argvv[i][j] );
        }
    }
}


/**
 * Get the command with its parameters for execvp
 * Execute this instruction before run an execvp to obtain the complete command
 * @param argvv
 * @param num_command
 * @return
 */
void getCompleteCommand(char*** argvv, int num_command) {
	//reset first
	for(int j = 0; j < 8; j++)
		argv_execvp[j] = NULL;

	int i = 0;
	for ( i = 0; argvv[num_command][i] != NULL; i++)
		argv_execvp[i] = argvv[num_command][i];
}


/**
 * Main sheell  Loop  
 */
int main(int argc, char* argv[])
{
	/**** Do not delete this code.****/
	int end = 0; 
	int executed_cmd_lines = -1;
	char *cmd_line = NULL;
	char *cmd_lines[10];

	if (!isatty(STDIN_FILENO)) {
		cmd_line = (char*)malloc(100);
		while (scanf(" %[^\n]", cmd_line) != EOF){
			if(strlen(cmd_line) <= 0) return 0;
			cmd_lines[end] = (char*)malloc(strlen(cmd_line)+1);
			strcpy(cmd_lines[end], cmd_line);
			end++;
			fflush (stdin);
			fflush(stdout);
		}
	}

	/*********************************/

	char ***argvv = NULL;
	int num_commands;

	history = (struct command*) malloc(history_size *sizeof(struct command));
	int run_history = 0;

	while (1) 
	{
		int status = 0;
		int command_counter = 0;
		int in_background = 0;
		signal(SIGINT, siginthandler);

		if (run_history)
    {
        run_history=0;
    }
    else{
        // Prompt 
        write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));

        // Get command
        //********** DO NOT MODIFY THIS PART. IT DISTINGUISH BETWEEN NORMAL/CORRECTION MODE***************
        executed_cmd_lines++;
        if( end != 0 && executed_cmd_lines < end) {
            command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
        }
        else if( end != 0 && executed_cmd_lines == end)
            return 0;
        else
            command_counter = read_command(&argvv, filev, &in_background); //NORMAL MODE
    }
		//************************************************************************************************


		/************************ STUDENTS CODE ********************************/
        /*
        Primeramente, crearemos un flag que se encarga únicamente de distinguir
        si se desea re-ejecutar un comando del historial o no. Esto es de utilidad
        para que el comando myhistory pueda ejecutarse en el mismo bloque de código
        que el resto de comandos, ya que de lo contrario, se ejecutaría en un bloque
        diferente y no se podría re-ejecutar un comando del historial.
        */
        int run_history_command = 0;

        if (command_counter > MAX_COMMANDS){
            perror("Error: Max number of commands exceeded\n");
            exit(EXIT_FAILURE);
        }
        if (command_counter == 1 && strcmp(argvv[0][0], "mycalc") == 0){
            if (in_background == 0){
				mycalc(argvv[0]);
                // Llamada a store_command para almacenar el comando actual en el historial
                store_command(argvv, filev, in_background, &history[tail]);

                // Actualizar el índice tail para apuntar al próximo espacio libre
                tail = (tail + 1) % history_size;

                // Ajustar el número de elementos y el índice head si es necesario
                if
                (n_elem < history_size) {
                    n_elem++;
                }
                else {
                    // Liberar la memoria del comando más antiguo
                    free_command(&history[head]);
                    head = (head + 1) % history_size; // El historial está lleno, avanzar head
                }
			}
			else{
				printf("Error: mycalc no se puede ejecutar en background \n");
			}
        }
        if(command_counter == 1 && strcmp(argvv[0][0], "myhistory") == 0){
            /*
            A diferencia de mycalc, myhistory no puede ejecutarse en una función auxiliar,
            ya que es posible que se desee re-ejecutar un comando del historial, lo que hace 
            necesario que se ejecute en el mismo bloque de código.
            */
            if (in_background == 0){
				if (argvv[0][1] == NULL) {
                    // Mostrar historial completo
                    int current = head;
                    for (int i = 0; i < n_elem; i++) {
                        // Verificar y mostrar la redirección de entrada
                        if (strcmp(history[current].filev[0], "0") != 0) {
                            fprintf(stderr, "< %s ", history[current].filev[0]);
                        }
                        fprintf(stderr, "%d ", i);
                        // Iterar sobre todos los comandos de la entrada actual del historial
                        for (int j = 0; j <= history[current].num_commands; j++) {
                            for (int k = 0; k < history[current].args[j]; k++) {
                                // Imprimir cada argumento del comando
                                fprintf(stderr, "%s ", history[current].argvv[j][k]);
                            }
                            if (j < history[current].num_commands - 1) {
                                fprintf(stderr, "| "); // Separar comandos en la misma entrada con un pipe
                            }
                        }
                        // Verificar y mostrar la redirección de salida
                        if (strcmp(history[current].filev[1], "0") != 0) {
                            fprintf(stderr, "> %s ", history[current].filev[1]);
                        }
                        if (history[current].in_background) {
                            fprintf(stderr, "&"); // Indicar si el comando se ejecutó en background
                        }
                        fprintf(stderr, "\n");
                        current = (current + 1) % history_size;
                    }
                } 
                
                else {
                    // Intentar ejecutar un comando específico del historial
                    int index = atoi(argvv[0][1]);
                    if (index < 0 || index >= n_elem) {
                        printf("ERROR: Comando no encontrado\n");
                    }

                    else {
                        /*Modificamos el flag*/
                        run_history_command = 1;

                        int actualIndex = (head + index) % history_size;
                        fprintf(stderr, "Ejecutando el comando %d\n", index);

                        /*
                        Modificaremos command_counter, filev, in_background y argvv 
                        para que contengan los valores del comando almacenado en el historial
                        y pasen así a ejecutarse en el siguiente bloque de código para que vuelva 
                        a introducirse en el historial.
                        */

                        // Asignar valores del comando seleccionado para su re-ejecución
                        command_counter = history[actualIndex].num_commands;
                        in_background = history[actualIndex].in_background;
                        // Asignar redirecciones de archivo
                        for (int f = 0; f < 3; f++) {
                            strcpy(filev[f], history[actualIndex].filev[f]);
                        }

                        // Asignar comandos y argumentos
                        for (int i = 0; i <= history[actualIndex].num_commands; i++) {
                            // Asignar número de argumentos para el comando actual
                            int num_args = history[actualIndex].args[i];
                            argvv[i] = (char **)calloc(num_args + 1, sizeof(char *));
                            
                            for (int j = 0; j < num_args; j++) {
                                argvv[i][j] = strdup(history[actualIndex].argvv[i][j]);
                            }
                            // El último argumento debe ser NULL para indicar el fin de los argumentos
                            argvv[i][num_args] = NULL;
                        }

                        // Asegúrate de que el siguiente comando en argvv sea NULL para indicar el fin de los comandos
                        argvv[history[actualIndex].num_commands + 1] = NULL;
                    }
                }
			}
			else{
				printf("Error: myhistory no se puede ejecutar en background \n");
			}
        }
        if((command_counter > 0 && strcmp(argvv[0][0], "mycalc") != 0 && strcmp(argvv[0][0], "myhistory") != 0) || run_history_command == 1){
            // Almacenamos el comando actual en el historial
            store_command(argvv, filev, in_background, &history[tail]);

            // Actualizar el índice tail para apuntar al próximo espacio libre
            tail = (tail + 1) % history_size;

            // Ajustar el número de elementos y el índice head si es necesario
            if
            (n_elem < history_size) {
                n_elem++;
            }
            else {
                // Liberar la memoria del comando más antiguo
                free_command(&history[head]);
                head = (head + 1) % history_size; // El historial está lleno, avanzar head
            }

            int i;
            int pipefd[2 * (command_counter - 1)]; // Cada comando puede necesitar un pipe para conectarse al siguiente

            // Crear todos los pipes necesarios de antemano
            for (i = 0; i < command_counter - 1; i++) {
                if (pipe(pipefd + i*2) < 0) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
            }

            for (i = 0; i < command_counter; i++) {
                pid_t pid = fork();
                if (pid == 0) { // Proceso hijo
                    // Si no es el primer comando, conectar la entrada estándar al pipe anterior
                    if (i > 0) {
                        dup2(pipefd[(i - 1) * 2], STDIN_FILENO);
                    }

                    // Si no es el último comando, conectar la salida estándar al siguiente pipe
                    if (i < command_counter - 1) {
                        dup2(pipefd[i * 2 + 1], STDOUT_FILENO);
                    }

                    // Cerrar todos los descriptores de archivo de pipes, ya no son necesarios
                    for (int j = 0; j < 2 * (command_counter - 1); j++) {
                        close(pipefd[j]);
                    }

                    if (i == command_counter - 1 && strcmp(filev[1], "0") != 0) {
                        int fd_out = open(filev[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                        if (fd_out == -1) {
                            perror("Error opening output file");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd_out, STDOUT_FILENO);
                        close(fd_out);
                    }

                    if (i == 0 && strcmp(filev[0], "0") != 0) {
                        int fd_in = open(filev[0], O_RDONLY);
                        if (fd_in == -1) {
                            perror("Error opening input file");
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd_in, STDIN_FILENO);
                        close(fd_in);
                    }

                    // Ejecutar el comando
                    execvp(argvv[i][0], argvv[i]);
                    // Si execvp devuelve algún valor, hubo un error
                    perror("execvp");
                    exit(EXIT_FAILURE);
                } else if (pid < 0) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
            }

            if (!in_background) {
                // El proceso padre cierra todos los descriptores de archivo de pipes, ya no son necesarios
                for (i = 0; i < 2 * (command_counter - 1); i++) {
                    close(pipefd[i]);
                }

                // Esperar a que todos los procesos hijos terminen
                for (i = 0; i < command_counter; i++) {
                    wait(NULL);
                }
            }
        }
	}
	
	return 0;
}

int mycalc(char *argv[]) {
    // Verificar que se pasaron los argumentos correctamente
    if (argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
        printf("[ERROR] La estructura del comando es mycalc <operando 1> <add/mul/div> <operando 2>\n");
        return -1;
    }

    int op1 = atoi(argv[1]);
    int op2 = atoi(argv[3]);
    int resultado;
    char *operacion = argv[2];

    // Obtener valor actual de Acc
    char *acc_str = getenv("Acc");
    int acc = (acc_str != NULL) ? atoi(acc_str) : 0;

    if (strcmp(operacion, "add") == 0) {
        resultado = op1 + op2;
        acc += resultado;
        // Actualizar el valor de Acc
        char acc_buffer[20];
        snprintf(acc_buffer, 20, "%d", acc);
        setenv("Acc", acc_buffer, 1);
        fprintf(stderr, "[OK] %d + %d = %d; Acc %d\n", op1, op2, resultado, acc);
    }
    
    else if (strcmp(operacion, "mul") == 0) {
        resultado = op1 * op2;
        fprintf(stderr, "[OK] %d * %d = %d\n", op1, op2, resultado);
    } 
    
    else if (strcmp(operacion, "div") == 0) {
        if (op2 == 0) {
            printf("[ERROR] División por cero.\n");
            return -1;
        }
        int cociente = op1 / op2;
        int resto = op1 % op2;
        fprintf(stderr, "[OK] %d / %d = %d; Resto %d\n", op1, op2, cociente, resto);
    }

    else {
        printf("[ERROR] La estructura del comando es mycalc <operando_1> <add/mul/div> <operando_2>\n");
        return -1;
    }

    return 0;
}

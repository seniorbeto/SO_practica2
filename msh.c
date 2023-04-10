//P2-SSOO-22/23

// MSH main file
// Write your msh source code here

//#include "parser.h"
#include <stddef.h>         /* NULL */
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
#include <time.h>
#include <pthread.h>

// Está limitado a ocho pero nuestra implementación contempla un
// número ilimitado de comandos
#define MAX_COMMANDS 8


// ficheros por si hay redirección
char filev[3][64];

//to store the execvp second parameter
char *argv_execvp[8];


void siginthandler(int param)
{
	printf("****  Saliendo del MSH **** \n");
	//signal(SIGINT, siginthandler);
	exit(0);
}

/* Timer */
pthread_t timer_thread;
unsigned long  mytime = 0;

void* timer_run ( )
{
	while (1)
	{
		usleep(1000);
		mytime++;
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

	pthread_create(&timer_thread,NULL,timer_run, NULL);

	/*********************************/

	char ***argvv = NULL;
	int num_commands;


	while (1) 
	{
		int status = 0;
		int command_counter = 0;
		int in_background = 0;
		signal(SIGINT, siginthandler);

		// Prompt 
		write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));

		// Get command
		//********** DO NOT MODIFY THIS PART. IT DISTINGUISH BETWEEN NORMAL/CORRECTION MODE***************
		executed_cmd_lines++;
		if( end != 0 && executed_cmd_lines < end) {
			command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
		}
		else if( end != 0 && executed_cmd_lines == end){
			return 0;
		}
		else{
			command_counter = read_command(&argvv, filev, &in_background); //NORMAL MODE
		}
		//************************************************************************************************
		
		/************************ STUDENTS CODE ********************************/
		if (command_counter > 0) {
			if (command_counter > MAX_COMMANDS){
				printf("Error: Número máximo de comandos es %d \n", MAX_COMMANDS);
			}
			else {
				// Print command
				//print_command(argvv, filev, in_background);

				// Ahora, creamos tantos procesos como la variable command_counter indique
				int pid;
				int i = 0;
				int indicador;
				if (command_counter > 1) {indicador = 1;} else {indicador = 0;};
				int fd[command_counter-1];
				pid = fork();
				if (pid < 0){
					perror("Error en la creación del proceso");
				}
				while (i < command_counter && pid == 0){
					// Estás en el hijo
					i++;	
						
					// Generamos las tuberías necesarias
					if (indicador == 1){
						if (pipe(fd) < 0){
							perror("Error de creación de tubería");
						}
					}

					// Si quedan mandatos por ejecutar, creamos otro hijo
					if (i != command_counter){
						pid = fork();
						if (pid < 0){
							perror("Error en la creación del proceso");
						}
					}
					// El padre esperará a sus hijos 
					if (pid > 0 || i == command_counter){
						// Si el proceso se ejecuta en Background, se imprime su pid
						if (in_background == 1){
							printf("%d\n", getpid());
						}
						// Ahora, ejecutamos la instrucción introducida como parámetro
						int j = 0;
						printf("Se va a ejecutar: %s. El pid de su padre es: %d\n", argvv[command_counter-i][j], getppid());
						wait(NULL);
						// Vamos de atrás a delante para que se ejecuten primero los primeros comandos
						while (argvv[command_counter-i][j] != NULL){
							if (execvp(argvv[command_counter-i][j], argvv[command_counter-i]) == -1){
								perror("Error en la ejecución del mandato");
							}
							j++;
						}
						// Finalmente, terminamos el proceso <-------------------------
						return(status);
					}
				}
			
				// Ahora esperamos a que todos los hijos terminen de ejecutarse
				int j;
			    for (j=0;j<command_counter;j++){
			        wait(NULL);
			    }
			}
		}
	}
	
	return 0;
}
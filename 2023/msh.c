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

// Prototipos
int mycalc(char *argvv[]);
int mytimer();


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
		if (command_counter > MAX_COMMANDS){
				printf("Error: Número máximo de comandos es %d \n", MAX_COMMANDS);
				return(-1);
		}
		else if (command_counter > 0 && strcmp(argvv[0][0], "mycalc") == 0){
			if (in_background == 0){
				mycalc(argvv[0]);
			}
			else{
				printf("Error: mycalc no se puede ejecutar en background \n");
			}
			
		}
		else if (command_counter > 0 && strcmp(argvv[0][0], "mytime") == 0){
			if (in_background == 0){
				mytimer();
			}
			else{
				printf("Error: mytime no se puede ejecutar en background \n");
			}
		}
		else if (command_counter > 0) {

				// En este caso, creamos tantos procesos como la variable command_counter 
				// indique
				int pid;
				int i = 0;
				
				// Generamos el proceso para el primer comando
				pid = fork();
				if (pid < 0){
					perror("Error en la creación del proceso");
					return(-1);
				}
				
				int fd[command_counter-1][2];
				// Estás en el hijo
				while (i < command_counter && pid == 0){

					// Generamos las tuberías necesarias
					if(pipe(fd[i]) < 0){
							perror("Error en la creación de las tuberías");
							return(-1);
						}
				
					i++;
					
					// Si quedan mandatos por ejecutar, creamos otro hijo
					if (i != command_counter){
						pid = fork();
						if (pid < 0){
							perror("Error en la creación del proceso");
							return(-1);
						}
					}

					// El padre y el último hijo ejecutarán los comandos en orden inverso
					// (el último proceso es el primer comando y el primero el último)
					if (pid > 0 || i == command_counter){
						
						// Si el proceso se ejecuta en Background, se imprime el pid del último
						// mandato de la secuencia
						if (in_background == 1 && i == 1){
							printf("[%d]\n", getpid());
						}
						
						// El padre esperará a sus hijos 
						wait(NULL);

						// Si es el último proceso, es decir, el primer comando no cambiaremos
						// su entrada
						if (i != command_counter){
							// De los que no son el último, cambiamos su entrada
							dup2(fd[i-1][0], STDIN_FILENO);
							close(fd[i-1][1]);	
						}
						// Redirección de entrada (último proceso = primer mandato)
						if (i == command_counter && strcmp(filev[0], "0") != 0){
							int fd_in = open(filev[0], O_RDONLY);
							if(fd_in < 0){
								perror("Error en la apertura de la redirección de entrada");
								return(-1);
							}
							dup2(fd_in, STDIN_FILENO);
							close(fd_in);
						}

						// Si es el primer proceso, es decir, el último comando, no cambiaremos 
						// su salida. 
						if(i != 1){
							// Del resto, cambiamos su salida
							dup2(fd[i-2][1], STDOUT_FILENO);
							close(fd[i-2][0]);
						}
						// Redirección de salida (primer proceso = último mandato)
						if (i == 1 && strcmp(filev[1], "0") != 0){
							int fd_out = open(filev[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
							if(fd_out < 0){
								perror("Error en la apertura de la redirección de salida");
								return(-1);
							}
							dup2(fd_out, STDOUT_FILENO);
							close(fd_out);
						}

						// Redirección de errores (primer proceso = último mandato)
						if (i == 1 && strcmp(filev[2], "0") != 0) {
							int fd_err = open(filev[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
							if (fd_err < 0) {
								perror("Error en la apertura de la redirección de errores");
								return(-1);
							}
							dup2(fd_err, STDERR_FILENO);
							close(fd_err);
						}
						
						// Vamos de atrás a delante para que se ejecuten primero los primeros comandos
						execvp(argvv[command_counter-i][0], argvv[command_counter-i]);
						perror("Error en la ejecución del mandato");
						return(-1);
					}
					
				}
				// Esperamos a que terminen los procesos.
				// En caso de que sea en background la minishell no debe bloquearse
				if (in_background == 0) {
					wait(NULL);
				}
			}
	}
	return 0;
}


// Mandato interno mycalc (add, mul, div):
int mycalc(char *argvv[]){

	// El mandato debe obtener exactamente 4 argumentos
	if(argvv[1] == NULL || argvv[2] == NULL || argvv[3] == NULL || argvv[4] != NULL){
		printf("[ERROR] La estructura del comando es mycalc <operando_1> <add/mul/div> <operando_2>\n");
		return -1;
	}
	
	// El mandato debe recibir el primer argumento como 'mycalc', los 
	// operandos como numeros y el operador solo puede ser add, mul y div
	if((strcmp(argvv[0], "mycalc") != 0) || 
		(atoi(argvv[1]) == 0  && strcmp(argvv[1], "0") != 0)|| 
		((strcmp(argvv[2], "add") != 0) && (strcmp(argvv[2], "mul") != 0) && (strcmp(argvv[2], "div") != 0))|| 
		(atoi(argvv[3]) == 0 && strcmp(argvv[3], "0") != 0)){

		printf("[ERROR] La estructura del comando es mycalc <operando_1> <add/mul/div> <operando_2>\n");
		return -1;
	}
	
	long long int Resultado;
	// Realizamos la suma
	if(strcmp(argvv[2], "add") == 0){
		long long int valor;
		// Usamos atoll para poder realizar operaciones con más cifras
		Resultado = atoll(argvv[1]) + atoll(argvv[3]);
		// Actualizamos el valor de Acc 
		if (getenv("Acc") == NULL){
			valor = 0;
		}
		else{
			valor = atoll(getenv("Acc"));
		}
		valor += Resultado;

		// Cada caracter ocupa 1B
		int digitos = 0;
		int n = valor;
		while (n != 0){
			digitos++;
			n /= 10;
		}
		char str_valor[digitos];
		// Lo pasamos a string
		sprintf(str_valor,"%lld", valor);

		// Creamos la variable de entorno Acc que guardará los resultados de las
		// sumas
		if(setenv("Acc", str_valor, 1) != 0){
			printf("[ERROR] Fallo en la creación de la variable de entorno");
			return -1;
		}
		
		// Mostramos resultado por la salida estándar de error
		fprintf(stderr, "[OK] %s + %s = %lld; Acc %s\n", argvv[1], argvv[3], Resultado, getenv("Acc"));
	
	}
	// Realizamos la multiplicación
	else if(strcmp(argvv[2], "mul") == 0){
		// Usamos atoll para poder realizar operaciones con más cifras
		Resultado = atoll(argvv[1]) * atoll(argvv[3]);
		// Mostramos resultado por la salida estándar de error
		fprintf(stderr, "[OK] %s * %s = %lld\n", argvv[1], argvv[3], Resultado);
	}
	// Realizamos la división
	else{
		if(atoll(argvv[3]) == 0){
			printf("[ERROR] No se puede dividir por 0\n");
			return -1;
		}
		long long int Resto;
		long long int Cociente;
		Cociente = atoll(argvv[1]) / atoll(argvv[3]);
		Resto = atoll(argvv[1]) % atoll(argvv[3]);
		// Mostramos resultado por la salida estándar de error
		fprintf(stderr, "[OK] %s / %s = %lld; Resto %lld\n", argvv[1], argvv[3], Cociente, Resto);
	}

	return 0;
}


// Mandato interno mytime: tiempo que lleva ejecutando la minishell en formato HH:MM:SS
int mytimer(){
	int horas;
	int minutos;
	int segundos;
	segundos = mytime/1000;
	if (segundos > 59){
		minutos = segundos/60;
		segundos -= (minutos*60);
		if (minutos > 59){
			horas = minutos/60;
			minutos -= (horas*60);
			fprintf(stderr, "%02d:%02d:%02d\n",horas, minutos, segundos);
		}
		else{
			fprintf(stderr, "00:%02d:%02d\n",minutos, segundos);				
		}
	}
	else{
		fprintf(stderr, "00:00:%02d\n", segundos);	
	}

	return 0;
}

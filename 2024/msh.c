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

/* myhistory */

/* myhistory */

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
        if (command_counter > MAX_COMMANDS){
            perror("Error: Max number of commands exceeded\n");
            exit(EXIT_FAILURE);
        }
        else if (command_counter > 0 && strcmp(argvv[0][0], "mycalc") == 0){
            printf("FALTA IMPLEMENTAR\n");
        }
        else if(command_counter > 0 && strcmp(argvv[0][0], "myhistory") == 0){
            printf("FALTA IMPLEMENTAR\n");
        }
        else if(command_counter > 0){
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
                    // Si execvp retorna, hubo un error
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

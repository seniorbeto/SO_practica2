
// P2-SSOO-23/24

//  MSH main file
// Write your msh source code here

// #include "parser.h"
#include <stddef.h> /* NULL */
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

// to store the execvp second parameter
char *argv_execvp[8];

void siginthandler(int param)
{
	printf("****  Exiting MSH **** \n");
	// signal(SIGINT, siginthandler);
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
struct command *history;
int head = 0;
int tail = 0;
int n_elem = 0;

void free_command(struct command *cmd)
{
	if ((*cmd).argvv != NULL)
	{
		char **argv;
		for (; (*cmd).argvv && *(*cmd).argvv; (*cmd).argvv++)
		{
			for (argv = *(*cmd).argvv; argv && *argv; argv++)
			{
				if (*argv)
				{
					free(*argv);
					*argv = NULL;
				}
			}
		}
	}
	free((*cmd).args);
}

void store_command(char ***argvv, char filev[3][64], int in_background, struct command *cmd)
{
	int num_commands = 0;
	while (argvv[num_commands] != NULL)
	{
		num_commands++;
	}

	for (int f = 0; f < 3; f++)
	{
		if (strcmp(filev[f], "0") != 0)
		{
			strcpy((*cmd).filev[f], filev[f]);
		}
		else
		{
			strcpy((*cmd).filev[f], "0");
		}
	}

	(*cmd).in_background = in_background;
	(*cmd).num_commands = num_commands - 1;
	(*cmd).argvv = (char ***)calloc((num_commands), sizeof(char **));
	(*cmd).args = (int *)calloc(num_commands, sizeof(int));

	for (int i = 0; i < num_commands; i++)
	{
		int args = 0;
		while (argvv[i][args] != NULL)
		{
			args++;
		}
		(*cmd).args[i] = args;
		(*cmd).argvv[i] = (char **)calloc((args + 1), sizeof(char *));
		int j;
		for (j = 0; j < args; j++)
		{
			(*cmd).argvv[i][j] = (char *)calloc(strlen(argvv[i][j]), sizeof(char));
			strcpy((*cmd).argvv[i][j], argvv[i][j]);
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
void getCompleteCommand(char ***argvv, int num_command)
{
	// reset first
	for (int j = 0; j < 8; j++)
		argv_execvp[j] = NULL;

	int i = 0;
	for (i = 0; argvv[num_command][i] != NULL; i++)
		argv_execvp[i] = argvv[num_command][i];
}

/**
 * Main sheell  Loop
 */
int main(int argc, char *argv[])
{
	/**** Do not delete this code.****/
	int end = 0;
	int executed_cmd_lines = -1;
	char *cmd_line = NULL;
	char *cmd_lines[10];

	if (!isatty(STDIN_FILENO))
	{
		cmd_line = (char *)malloc(100);
		while (scanf(" %[^\n]", cmd_line) != EOF)
		{
			if (strlen(cmd_line) <= 0)
				return 0;
			cmd_lines[end] = (char *)malloc(strlen(cmd_line) + 1);
			strcpy(cmd_lines[end], cmd_line);
			end++;
			fflush(stdin);
			fflush(stdout);
		}
	}

	/*********************************/

	char ***argvv = NULL;
	int num_commands;

	history = (struct command *)malloc(history_size * sizeof(struct command));
	int run_history = 0;

	while (1)
	{
		int status = 0;
		int command_counter = 0;
		int in_background = 0;
		signal(SIGINT, siginthandler);

		if (run_history)
		{
			run_history = 0;
		}
		else
		{
			// Prompt
			write(STDERR_FILENO, "MSH>>", strlen("MSH>>"));

			// Get command
			//********** DO NOT MODIFY THIS PART. IT DISTINGUISH BETWEEN NORMAL/CORRECTION MODE***************
			executed_cmd_lines++;
			if (end != 0 && executed_cmd_lines < end)
			{
				command_counter = read_command_correction(&argvv, filev, &in_background, cmd_lines[executed_cmd_lines]);
			}
			else if (end != 0 && executed_cmd_lines == end)
				return 0;
			else
				command_counter = read_command(&argvv, filev, &in_background); // NORMAL MODE
		}
		//************************************************************************************************

		/************************ STUDENTS CODE ********************************/

		if (getenv("Acc") == NULL)
		{
			setenv("Acc", "0", 1);
		}
		struct command test;

		store_command(argvv, filev, in_background, &test);
		// printf("hola %s \n", test.args);
		// Mycalc
		if (strcmp(argvv[0][0], "mycalc") == 0)
		{
			int op1 = atoi(argvv[0][1]);
			int op2 = atoi(argvv[0][3]);
			char output[100];
			if (((strcmp(argvv[0][2], "add") == 0) || (strcmp(argvv[0][2], "div") == 0) || (strcmp(argvv[0][2], "mul") == 0)) // comprobamos que el segundo argumento es o sum o mul o div
				&& ((sizeof(op1) == sizeof(int)) && (sizeof(op2) == sizeof(int))))
			{
				int res = 0;
				if ((strcmp(argvv[0][2], "add")) == 0)
				{
					res = op1 + op2;
					char *acc_backup = getenv("Acc");
					int acc_int_backup = atoi(acc_backup);
					acc_int_backup += res;
					char numeroComoChar[20];
					sprintf(numeroComoChar, "%d", acc_int_backup);
					setenv("Acc", numeroComoChar, 1);
					snprintf(output, 100, "[OK] %d + %d = %d; Acc %d\n", op1, op2, res, acc_int_backup);
					write(STDERR_FILENO, output, strlen(output));
				}
				else if ((strcmp(argvv[0][2], "div") == 0))
				{
					int resto = 0;
					res = op1 / op2;
					resto = op1 % op2;
					snprintf(output, 100, "[OK] %d / %d = %d; Resto %d\n", op1, op2, res, resto);
					write(STDERR_FILENO, output, strlen(output));
				}
				else
				{
					res = op1 * op2;
					snprintf(output, 100, "[OK] %d * %d = %d\n", op1, op2, res);
					write(STDERR_FILENO, output, strlen(output));
				}
			}
			else
			{
				snprintf(output, 100, "[ERROR] La estructura del comando es mycalc <operando_1> <add/mul/div> <operando_2>\n");
				write(STDOUT_FILENO, output, strlen(output));
			}
		}

		else
		{

			pid_t pid;
			int entrada, salida, error;

			if (command_counter > 1)
			{
				int pipes[command_counter - 1][2];
				for (int f = 0; f < command_counter - 1; f++)
				{
					if (pipe(pipes[f]) == -1)
					{
						perror("Error en el pipe");
					};
				}

				for (int i = 0; i < command_counter; i++)
				{
					printf("Creando proceso...\n");
					pid = fork();
					if (pid == 0)
					{
						getCompleteCommand(argvv, i);
						printf("command_counter: %d\n", command_counter);
						printf("mi comando es %s\n", argv_execvp[0]);
						printf("%d\n", i);

						if (i == 0)
						{
							/*
							Estamos en el primer comando, por lo que únicamente necesitamos redirigir la salida estándar
							y cerraremos todo lo demás
							*/
							close(STDOUT_FILENO);
							dup(pipes[i][1]);
							close(pipes[i][1]);
						}
						else if (i == command_counter - 1)
						{
							/*
							Estamos en el último comando, por lo que únicamente necesitamos redirigir la entrada estándar
							y cerraremos todo lo demás
							*/
							close(STDIN_FILENO);
							dup(pipes[i - 1][0]);
							close(pipes[i - 1][0]);
						}
						else
						{
							close(pipes[i][0]);
							dup2(pipes[i][1], STDOUT_FILENO);
							close(pipes[i - 1][1]);
							dup2(pipes[i - 1][0], STDIN_FILENO);
						}

						execvp(argv_execvp[0], argv_execvp);
						perror("Error Execvp");
						exit(-1);
					}
				}

				if (in_background == 0)
				{
					while (wait(&status) != pid)
						;
					printf("Soy el padre\n");
				}

				else
				{
					break;
				}

				for (int j = 0; j < command_counter - 1; j++)
				{
					close(pipes[j][0]);
					close(pipes[j][1]);
				}
			}
			else
			{
				pid = fork();
				switch (pid)
				{
				case -1:
					perror("Error en el fork");
				case 0:
					if (strcmp(filev[0], "0") != 0)
					{
						close(STDIN_FILENO);
						entrada = open(filev[0], O_RDONLY, 0666);
					}
					if (strcmp(filev[1], "0") != 0)
					{
						close(STDOUT_FILENO);
						salida = open(filev[1], O_CREAT | O_WRONLY, 0666);
					}
					if (strcmp(filev[2], "0") != 0)
					{
						close(STDERR_FILENO);
						error = open(filev[2], O_CREAT | O_WRONLY, 0666);
					}
					getCompleteCommand(argvv, 0);
					execvp(argv_execvp[0], argv_execvp);
				default:
					if (in_background == 0)
					{
						wait(&status);
					}

					else
					{
						break;
					}
				}
			}
		}
	}
	return 0;
}
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

#define ANSI_COLOR_RED     "\x1b[31m"



//#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_GREEN   "\033[1;32m"

#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
//#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RESET   "\033[0m"

#define MAX 100


typedef struct job
{
    int job_id;
    pid_t pid;
    char cmd[100];
    int status;           // 0 = Running, 1 = Stopped
    struct job *next;
} job_t;


void scan_input();
char *get_command(char *input_string);

void copy_change(char *prompt, char *input_string);

int check_command_type(char *command);
void echo(char *input_string, int status);
void execute_internal_commands(char *input_string);
void signal_handler(int sig_num);
void extract_external_commands(char **external_commands);

void execute_external_commands(char *input_string);

void execute_no_command(char* input);
#endif
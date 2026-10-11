#include "main.h"


char command[50];
char input[MAX];
char prompt[100] = "Minishell$: ";
char *cmd;
pid_t pid = 0;
int status;
  


void signal_handler(int sig_num)
{
    if(sig_num == SIGINT)
    {
        if(pid == 0)
        {
            printf(ANSI_COLOR_GREEN"\n %s " ANSI_COLOR_RESET, prompt);
            fflush(stdout);
        }
    } 

    if(sig_num == SIGTSTP)
    {
        if(pid == 0)
        {
          //  printf(ANSI_COLOR_GREEN"\n %s " ANSI_COLOR_RESET, prompt);
            fflush(stdout);
        }
    }
}


char *get_command(char *input_string)
{
    static char cmd[50];
    int i = 0;

    while (input[i] != ' ' && input[i] != '\0')
    {
        cmd[i] = input[i];
        i++;
    }
    cmd[i] = '\0';

    return cmd;
}

int check_command_type(char *command)
{
    if (!strcmp(cmd, "exit") ||
        !strcmp(cmd, "pwd") ||
        !strcmp(cmd, "cd"))
        return BUILTIN;
    else if(strcmp(cmd, "fg") == 0 || strcmp(cmd, "bg") == 0 || strcmp(cmd, "jobs") == 0)
        return NO_COMMAND;
    
    return EXTERNAL;
}

void execute_internal_commands(char *input_string)
{
    if(strcmp(input_string, "exit") == 0)
    {
        exit(0);
    }
    else if(strcmp(input_string, "pwd") == 0)
    {
        char cwd[100];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
    }
    else if(strncmp(input_string, "cd", 2) == 0)
    {
        char *path;

        if(input_string[2] == ' ' && input_string[3] != '\0')
        {
            path = input_string + 3;
        }
        else
        {
            path = getenv("HOME");    /* plain "cd" goes to the home directory */
        }

        if(path == NULL || chdir(path) != 0)
        {
            perror("cd");
        }
    }
}


void execute_external_commands(char *input)
{
    char *cmds[10];
    int cmd_count = 0;

    cmds[cmd_count] = strtok(input, "|");
    while (cmds[cmd_count])
        cmds[++cmd_count] = strtok(NULL, "|");

    int fd[2];
    int in_fd = 0;

    for (int i = 0; i < cmd_count; i++)
    {
        pipe(fd);
        pid = fork();
        if (pid == 0)
        {
            dup2(in_fd, 0);

            if (i != cmd_count - 1)
                dup2(fd[1], 1);

            close(fd[0]);

            char *args[20];
            int j = 0;

            args[j] = strtok(cmds[i], " ");
            while (args[j])
                args[++j] = strtok(NULL, " ");

            //signal(SIGINT, SIG_DFL);
            //signal(SIGTSTP, SIG_DFL);

            execvp(args[0], args);
            perror("execvp");
            exit(1);
        }
        else
        {
            waitpid(pid, &status, WUNTRACED);
            pid = 0;
            close(fd[1]);
            in_fd = fd[0];
        }
    }
}

void execute_no_command(char* input)
{
    /* TODO: implement jobs, fg and bg (store jobs in a linked list of job_t) */
}

void scan_input(void)
{
    
    int cmd_type;

    while (1)
    {
        printf(ANSI_COLOR_GREEN" %s " ANSI_COLOR_RESET, prompt);
        fflush(stdout);

        if (fgets(input, MAX, stdin) == NULL)
        {
            /* EOF (Ctrl+D) or read error: leave the shell instead of looping */
            printf("\n");
            exit(0);
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0)
            continue;

        /* PS1 handling */
        if (strncmp(input, "PS1=", 4) == 0)
        {
            strcpy(prompt, input + 4);
            continue;
        }

        cmd = get_command(input);

       // printf("%s \n", cmd);
        cmd_type = check_command_type(cmd);

       if (cmd_type == BUILTIN)
        {
            execute_internal_commands(input);
        } 
        else if ( cmd_type == EXTERNAL)
        {
            execute_external_commands(input);
        }
        else if( cmd_type == NO_COMMAND)
        {
            execute_no_command(input);
        }
    }
}
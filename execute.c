#include "minishell.h"

/* FULL BUILTIN LIST (as you gave) */
char *builtins[] = {
    "echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs",
    "let", "eval", "set", "unset", "export", "declare", "typeset",
    "readonly", "getopts", "source", "exit", "exec", "shopt",
    "caller", "true", "type", "hash", "bind", "help",
    "fg", "jobs", "bg", NULL};

/* GLOBALS */
int pid = 0;
int status;
node *head = NULL;


/* GET COMMAND */
char *get_command(char *input_string)
{
    int i = 0;

    while (input_string[i] != ' ' && input_string[i] != '\0')
        i++;

    input_string[i] = '\0';
    return input_string;
}

/* CHECK COMMAND TYPE */
int check_cmd_type(char *cmd)
{
    for (int i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(cmd, builtins[i]) == 0)
            return BUILTIN;
    }
    return EXTERNAL;
}

/* BUILTIN COMMANDS */
void execute_internal_commands(char *input_string)
{
    /* EXIT */
    if (strcmp(input_string, "exit") == 0)
    {
        exit(0);
    }

    /* PWD */
    else if (strcmp(input_string, "pwd") == 0)
    {
        char buffer[1024];
        getcwd(buffer, sizeof(buffer));
        printf("%s\n", buffer);
    }

    /* CD */
    else if (strncmp(input_string, "cd", 2) == 0)
    {
        if (strlen(input_string) == 2)
        {
            chdir(getenv("HOME"));
        }
        else
        {
            if (chdir(input_string + 3) != 0)
                perror("cd");
        }
    }

    /* ECHO $$ */
    else if (strcmp(input_string, "echo $$") == 0)
    {
        printf("%d\n", getpid());
    }

    /* ECHO $? */
    else if (strcmp(input_string, "echo $?") == 0)
    {
        if (WIFEXITED(status))
            printf("%d\n", WEXITSTATUS(status));
        else if (WIFSIGNALED(status))
            printf("%d\n", 128 + WTERMSIG(status));
    }

    /* ECHO $SHELL */
    else if (strcmp(input_string, "echo $SHELL") == 0)
    {
        printf("%s\n", getenv("SHELL"));
    }

    /* JOBS */
    else if (strcmp(input_string, "jobs") == 0)
    {
        print_list();
    }

    /* FG */
    else if (strcmp(input_string, "fg") == 0)
    {
        if (head == NULL)
        {
            printf("fg: current: no such job\n");
            return;
        }

        kill(head->pid, SIGCONT);
        waitpid(head->pid, &status, WUNTRACED);
        delete_first();
    }

    /* BG */
    else if (strcmp(input_string, "bg") == 0)
    {
        if (head == NULL)
        {
            printf("bg: current: no such job\n");
            return;
        }

        kill(head->pid, SIGCONT);
    }

    /* OTHER BUILTINS (NOT IMPLEMENTED) */
    else
    {
        printf("%s: builtin not implemented\n", input_string);
    }
}

/* EXTERNAL COMMANDS */
void execute_external_commands(char *input_string)
{
    char *argv[50];
    int argc = 0;

    char *token = strtok(input_string, " ");
    while (token != NULL)
    {
        argv[argc++] = token;
        token = strtok(NULL, " ");
    }
    argv[argc] = NULL;

    /* PIPE CHECK */
    int cmd_index[20], cmd_count = 0;
    cmd_index[cmd_count++] = 0;

    int pipe_present = 0;

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "|") == 0)
        {
            argv[i] = NULL;
            cmd_index[cmd_count++] = i + 1;
            pipe_present = 1;
        }
    }

    /* NO PIPE */
    if (!pipe_present)
    {
        pid = fork();

        if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);

            execvp(argv[0], argv);

            /* if exec fails */
            printf("%s: command not found\n", argv[0]);
            exit(1);
        }
        else
        {
            waitpid(pid, &status, WUNTRACED);
            pid = 0;
        }
        return;
    }

    /* PIPE HANDLING */
    int fd[2];
    int prev_fd = -1;

    for (int i = 0; i < cmd_count; i++)
    {
        if (i < cmd_count - 1)
        {
            if (pipe(fd) == -1)
            {
                perror("pipe");
                exit(1);
            }
        }

        int child = fork();

        if (child == 0)
        {
            if (prev_fd != -1)
            {
                dup2(prev_fd, 0);
                close(prev_fd);
            }

            if (i < cmd_count - 1)
            {
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
            }

            execvp(argv[cmd_index[i]], &argv[cmd_index[i]]);
            perror("exec");
            exit(1);
        }

        if (prev_fd != -1)
            close(prev_fd);

        if (i < cmd_count - 1)
        {
            close(fd[1]);
            prev_fd = fd[0];
        }
    }

    for (int i = 0; i < cmd_count; i++)
        wait(NULL);
}
#ifndef MINISHELL_H
#define MINISHELL_H

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
#include <errno.h>

typedef struct node
{
    int pid;
    char command[100];   // increased size (safe)
    struct node *next;
} node;

extern node *head;
extern int pid, status;

/* MACROS */
#define BUILTIN     1
#define EXTERNAL    2
#define NO_COMMAND  3

#define ANSI_COLOR_DARK_GREEN  "\033[32;1m"
#define ANSI_COLOR_RESET       "\x1b[0m"

/* FUNCTIONS */
void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);

int check_cmd_type(char *command);
void execute_internal_commands(char *input_string);
void execute_external_commands(char *input_string);

void signal_handler(int sig_num);

void insert_at_first(int pid, char *cmd);
void delete_first();
void print_list();

#endif
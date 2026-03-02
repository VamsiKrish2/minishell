#include "minishell.h"

extern node *head;

void insert_at_first(int pid, char *cmd)
{
    node *new = malloc(sizeof(node));
    if (!new)
    {
        printf("Memory error\n");
        return;
    }

    new->pid = pid;
    strncpy(new->command, cmd, sizeof(new->command));
    new->next = head;
    head = new;
}

void delete_first()
{
    if (head == NULL)
        return;

    node *temp = head;
    head = head->next;
    free(temp);
}

void print_list()
{
    node *temp = head;
    int job_no = 1;

    if (temp == NULL)
    {
        printf("No jobs\n");
        return;
    }

    while (temp)
    {
        printf("[%d] PID:%d CMD:%s\n", job_no++, temp->pid, temp->command);
        temp = temp->next;
    }
}
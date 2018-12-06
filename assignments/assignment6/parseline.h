#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef PARSELINE_H
#define PARSELINE_H

#define MAX_CMD_LENGTH 512
#define MAX_IN_CHAR_LENGTH 32
#define MAX_OUT_CHAR_LENGTH 32
#define PIPE_LIMIT 10
#define ARGUMENT_LIMIT 10
#define ONE_BYTE 8

struct Command {
    char command_line[MAX_CMD_LENGTH];
    int argc;
    char *argv[MAX_CMD_LENGTH];
    char input[MAX_IN_CHAR_LENGTH];
    char output[MAX_OUT_CHAR_LENGTH];
    struct Command *next_command;
    
};

int check_command_line(char *command_line);
void print_command(struct Command *cmd, int stage_number);
int parse_line(char *command_line, struct Command *cmd, int stage_number);
int parseline(char *command_line);

#endif
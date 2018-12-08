#define _BSD_SOURCE
#define _GNU_SOURCE
#define _DEFAULT_SOURCE 1

#ifndef _UNISTD_H
#include <unistd.h>
#endif

#ifndef _FCNTL_H
#include <fcntl.h>
#endif

#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef _STDLIB_H
#include <stdlib.h>
#endif

#ifndef _STRING_H
#include <string.h>
#endif

#ifndef _SIGNAL_H
#include <signal.h>
#endif

#ifndef _SYS_TYPES_H
#include <sys/types.h>
#endif

#ifndef _SYS_STAT_H
#include <sys/stat.h>
#endif

#ifndef _SYS_WAIT_H
#include <sys/wait.h>
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
    char *argv[PIPE_LIMIT];
    char input[MAX_IN_CHAR_LENGTH];
    char output[MAX_OUT_CHAR_LENGTH];
    struct Command *next_command;
    
};

int parseline(char *command_line);

#endif

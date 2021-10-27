#ifndef PARSELINE_H
#define PARSELINE_H

#define MAX_INPUTS 10;

struct Command {
    char *command_line;
    int argc;
    int *argv[MAX_INPUTS];
    char input;
    char output;
};

#endif
#include <string.h>
#include <ctype.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "parseline.h"

char *append_char(char *current_word, char *to_append) {
  char *new_name;
  char *append;

  /* Malloc new space for the new name with the correct size */
  new_name = malloc(sizeof(*new_name) + strlen(current_word) +
                    strlen(to_append) + 1);
  /* Copy the old name to new_name */
  strcpy(new_name, current_word);
  /* Set the the next space after the old name to be a pointer to the rest of
   * the name */
  append = new_name + strlen(current_word);
  /* copy the new portion of the name to the pointer */
  strcpy(append, to_append);
  return new_name;
}


int check_command_line(char *command_line, struct Command *cmd) {
    int i;
    int input_flag;
    int output_flag;
    int pipe_flag;

    if (strlen(command_line) > 512) {
        printf("command too long");
        exit(1);
    }
    for (i = 0; i < strlen(command_line); i++) {
        if (strcmp(command_line[i], '<')) {
            if (input_flag == 1) {
                printf("%s: bad input redirection", cmd->argv[0]);
                exit(1);
            }
            input_flag = 1;
        }
        if (strcmp(command_line[i], '>')) {
            if (output_flag == 1) {
                printf("%s: bad output redirection", cmd->argv[0]);
                exit(1);
            }
            output_flag = 1;
        }
        if (strcmp(command_line[i], '|')) {

            if (pipe_flag == 1) {
                printf("invalid null command");
                exit(1);
            }

            pipe_flag = 1;

            if (input_flag == 2) {
                printf("%s: ambiguous input", cmd->argv[0]);
                exit(1);
            }
            if (output_flag == 2) {
                printf("%s: ambiguous output", cmd->argv[0]);
                exit(1);
            }

            if (input_flag == 1) {
                input_flag = 2;
            }

            if (output_flag == 1) {
                output_flag = 2;
            }
        }
    }
    
}
void create_argv(char *command_line, struct Command *cmd) {
    int i;

    int input_flag = 0;
    int output_flag = 0;
    int **buffer;
    char *buffer_name = malloc(1);

    for (i = 0; i < strlen(command_line); i++) {
        if (input_flag == 1) {
            if (isspace(command_line[i])) {
                input_flag = 0;
            }
            continue;
        }

        if (output_flag == 1) {
            if (isspace(command_line[i])) {
                output_flag = 0;
            }
            continue;
        }

        if (strcmp(command_line[i], '<') || strcmp(command_line[i], '>') || strcmp(command_line[i], '|')) {

            swtich (command_line[i]) {
            case '<':
                input_flag = 1;
            case '>':
                output_flag = 1;
            }
            i++;
            continue;

        } else if (isspace(command_line[i]) || strcmp(command_line[i + 1], '\n')) {
            strncpy(cmd->argv[cmd->argc], buffer_name, strlen(buffer_name));
            cmd->argc += 1;
            free(buffer_name);
            buffer_name = malloc(1);
        } else {
            append_char(buffer_name, command_line[i]);
        }

    }
    free(buffer_name);
}

int get_command() {
    char command_line;
    struct Command *cmd = (struct Command*)calloc(1, sizeof(struct Command));
    
    printf("line: ");
    scanf("%s", command_line);
    cmd->command_line = command_line;
    cmd->argc = 0;
    cmd->argv = malloc(8 * 512);
    

    return 0;
}

int main(int argc, int **argv) {



}
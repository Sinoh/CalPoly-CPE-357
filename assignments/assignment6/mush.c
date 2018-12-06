#include "mush.h"
#include "parseline.h"


int main (int argc, char **argv) {
    char command_line[MAX_CMD_LENGTH];
    FILE *fp;
    size_t new_len;

    if (argc == 2) {
        if ((fp = fopen(argv[1], "r")) != NULL) {
            new_len = fread(command_line, sizeof(char), MAX_CMD_LENGTH, fp);
            if (ferror(fp) != 0 ) {
                fputs("Error reading file", stderr);
            } else {
                command_line[new_len++] = '\0';
                parseline(command_line);
            }
        fclose(fp);
        }
    } else {
        while(1) {
            printf("8-P ");
            if (fgets(command_line, MAX_CMD_LENGTH, stdin) == NULL ) {
                break;
            }

            parseline(command_line);
        }
    }
    printf("\n");
    return 0;
}


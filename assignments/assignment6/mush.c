#include "mush.h"
#include "parseline.h"

// SIGINT handler
void interrupt_handler() {
  // this will ignore the kill command the following line
  sigset(SIGINT, SIG_IGN);
  // kills children in same process group
  killpg(getpid(), SIGINT);
  // sets mush to catch SIGINT with this handler
  sigset(SIGINT, interrupt_handler);
}

int main(int argc, char **argv) {
  char command_line[MAX_CMD_LENGTH];
  char *token = NULL;
  char cpy[MAX_CMD_LENGTH];
  struct sigaction int_act;
  FILE *fp;
  size_t new_len;

  int_act.sa_handler = interrupt_handler;
  sigaction(SIGINT, &int_act, NULL);

  if (argc == 2) {
    /* If two aruments is give, open the file and run that command */
    if ((fp = fopen(argv[1], "r")) != NULL) {
      new_len = fread(command_line, sizeof(char), MAX_CMD_LENGTH, fp);
      if (ferror(fp) != 0) {
        fputs("Error reading file", stderr);
      } else {
        command_line[new_len++] = '\0';
        if ((strlen(command_line) > 0) &&
            (command_line[strlen(command_line) - 1] == '\n')) {
          command_line[strlen(command_line) - 1] = '\0';
        } else {
          fprintf(stderr, "command too long\n");
          exit(1);
        }

      parseline(command_line);
      fclose(fp);
    }
  } else {
    while (1) {
      printf("8-P ");
      // wait for input through stdin
      if (fgets(command_line, MAX_CMD_LENGTH, stdin) == NULL) {
        if (feof(stdin)) {
          break;
        }
        fprintf(stderr, "\n");
        continue;
      }
      if ((strlen(command_line) > 0) &&
          (command_line[strlen(command_line) - 1] == '\n')) {
        command_line[strlen(command_line) - 1] = '\0';
        if (strlen(command_line) == 0) {
          continue;
        }
      } else {
        fprintf(stderr, "command too long\n");
        continue;
      }
      /* Check if the command passed is cd */
      if (strstr(command_line, "cd") != NULL) {
        strcpy(cpy, command_line);
        token = strtok(cpy, " ");

        /* check if cd is missing an argument */
        if ((token = strtok(NULL, " ")) == NULL) {
          fprintf(stderr, "cd: missing argument.\n");
          continue;
        }
        /* Need to check if "too many arguments.\n" is passed */
        if (chdir(token) != 0) {
          perror("cd");
        }
        continue;
      }
      parseline(command_line);
    }
  }
  printf("\n");
  return 0;
}

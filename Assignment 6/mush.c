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
  char command_line_backup[MAX_CMD_LENGTH];
  struct sigaction int_act;
  FILE *fp;
  size_t new_len;
  int i;
  

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

      parseline(command_line);
      fclose(fp);
      }
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
      /* Removes newline '\n' if present */
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
        struct Command *cmd = (struct Command *)calloc(1, sizeof(struct Command));
        strcpy(cmd->command_line, command_line);
        strcpy(cmd->input, "original stdin");
        strcpy(cmd->output, "original stdout");
        cmd->argc = 0;
        for (i = 0; i < PIPE_LIMIT; i++) {
          cmd->argv[i] = calloc(1, MAX_CMD_LENGTH);
        }
        
        strcpy(command_line_backup, command_line);
        parse_line(command_line_backup, cmd, 0);

        /* if cd is missing an argument */
        if (cmd->argc == 1) {
          fprintf(stderr, "cd: missing arguments.\n");
          continue;
        }
        if (cmd->argc > 2) {
          fprintf(stderr, "cd: too many argument.\n");
          continue;
        }

        /* Need to check if "too many arguments.\n" is passed */
        if (chdir(cmd->argv[1]) != 0) {
          perror("cd");
        }
        free_cmd_struct(cmd);
        continue;
      }
      parseline(command_line);
    }
  }
  printf("\n");
  return 0;
}

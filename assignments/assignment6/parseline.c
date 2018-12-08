#include "parseline.h"

/* Call to check for error cases of the given command_line from scanf */
int check_command_line(char *command_line) {
  char cpy[MAX_CMD_LENGTH] = {0};
  char command[MAX_CMD_LENGTH] = {0};
  int argc, input_flag, input_flag_arg, output_flag, output_flag_arg, 
        pipes, pipe_input, pipe_output, pipe_flag_arg;
  char *token;

  argc = input_flag = input_flag_arg = output_flag = output_flag_arg = 
        pipes = pipe_input = pipe_output = pipe_flag_arg = 0;

  /* Copies the command_line to a temp */
  strcpy(cpy, command_line);

  /* seperate temp by spaces*/
  token = strtok(cpy, " ");

  /* Loops through each word*/
  while (token != NULL) {
    //Checks if there are too many arguments
    if (argc > ARGUMENT_LIMIT) {
      fprintf(stderr, "Too many arguments\n");
      return 1;
    }

    //Checks if there are too many stages
    if (pipes > PIPE_LIMIT) {
      fprintf(stderr, "Pipeline too deep\n");
      return 1;
    }

    //Checks if the token is an input
    if (!strcmp(token, "<")) {
      //if input hasn't been set then set the flag and argument flag
      if (input_flag == 0) {
        input_flag = 1;
        input_flag_arg = 1;
        //if input is after a pipe than it is considered a command
        if (pipe_flag_arg == 1) {
          memset(command, '\0', MAX_CMD_LENGTH);
          strcpy(command, token);
        }
        //if input flag arg is on then it is a bad redirection
      } else if (input_flag_arg == 1) {
        fprintf(stderr, "%s: bad input redirection\n", command);
        return 1;
        //if output flag arg is on then it is also a bad redirection
      } else if (output_flag_arg == 1) {
        fprintf(stderr, "%s: bad output redirection\n", command);
        return 1;
        //If input flag not 0 than it has more than 1 input to the same command
      } else {
        fprintf(stderr, "%s: bad input redirection\n", command);
        return 1;
      }
      // if the input char is after a pipe than it is ambigiuous input
      if ((pipe_output + input_flag) == 2) {
        fprintf(stderr, "%s: ambiguous input\n", command);
        return 1;
      }
    }

    //checks if the token is an output
    else if (!strcmp(token, ">")) {
      //if output has not been set then set it and the arg flag
      if (output_flag == 0) {
        output_flag = 1;
        output_flag_arg = 1;
        //if after a pipe it is considered a command
        if (pipe_flag_arg == 1) {
          memset(command, '\0', MAX_CMD_LENGTH);
          strcpy(command, token);
        }
        //if after an input char it is a bad redirection
      } else if (input_flag_arg == 1) {
        fprintf(stderr, "%s: bad input redirection\n", command);
        return 1;
        //if after an output char it is a bad redirection
      } else if (output_flag_arg == 1) {
        fprintf(stderr, "%s: bad output redirection\n", command);
        return 1;
        //if output flag is set than more than 1 output to same command
      } else {
        fprintf(stderr, "%s: bad output redirection\n", command);
        return 1;
      }
    }

    //checks if the token is a pipe
    else if (!strcmp(token, "|")) {
      //if any flag arg is set than bad redirection
      if (input_flag_arg == 1) {
        fprintf(stderr, "%s: bad input redirection\n", command);
        return 1;
      } else if (output_flag_arg == 1) {
        fprintf(stderr, "%s: bad output redirection\n", command);
        return 1;
        //if an output is already set than it is ambiguous
      } else if (output_flag == 1) {
        fprintf(stderr, "%s: ambiguous output\n", command);
        return 1;
        //if a pipe is set after a pipe is it a null command
      } else if (pipe_flag_arg == 1) {
        fprintf(stderr, "invalid null command\n");
        return 1;
        //if there is no command than null command
      } else if (command[0] == '\0') {
        fprintf(stderr, "invalid null command\n");
        return 1;
        //if pipe not set than set pipe flags and set input and argc to 0
      } else {
        pipe_flag_arg = 1;
        pipe_input = 1;
        input_flag = 0;
        pipe_output = 1;
        pipes++;
        argc = 0;
      }
      //if command outputs to a file and outputs to a pipe than ambiguous
      if ((pipe_input + output_flag) == 2) {
        fprintf(stderr, "%s: ambiguous output\n", command);
        return 1;
      }
      //if command inputs from a file and reads from a pipe than ambiguous
      if ((pipe_output + input_flag) == 2) {
        fprintf(stderr, "%s: ambiguous input\n", command);
        return 1;
      }
      //token is a command or argument
    } else {
      //token is an input file
      if (input_flag_arg == 1) {
        input_flag_arg = 0;
        //token is an output file
      } else if (output_flag_arg == 1) {
        output_flag_arg = 0;
        //token is the new command for next stage
      } else if (pipe_flag_arg == 1) {
        memset(command, '\0', sizeof(MAX_CMD_LENGTH));
        strcpy(command, token);
        argc++;
        pipe_flag_arg = 0;
      } else {
        //if command is empty than token is 1st command
        if (command[0] == '\0') {
          strcpy(command, token);
        }
        argc++;
      }
    }

    token = strtok(NULL, " ");

  }

  //if any flag_arg remains than the respective error is called
  if (input_flag_arg == 1) {
    fprintf(stderr, "%s: bad input redirection\n", command);
    return 1;
  } else if (output_flag_arg == 1) {
    fprintf(stderr, "%s: bad output redirection\n", command);
    return 1;
  } else if (pipe_flag_arg == 1) {
    fprintf(stderr, "invalid null command\n");
    return 1;
  }

  return 0;
}

/* Does the process control (fork, exec, and file redirection) for
   each command*/
void run_command(struct Command *cmd, int stage_number) {
   int stages, pipes, i, j, in_fd, out_fd, fd[20]; //MAKE 20 A #DEFINE 
   struct Command *temp = NULL;

   stages = pipes = i = j = in_fd = out_fd = 0;
   temp = cmd;
   
   //create input file redirection for first command
   if ((strcmp(temp->input, "original stdin")) && (strcmp(temp->input, "pipe from stage 0"))) {
      //fprintf(stderr, "creating a read only fd for input\n");
      if ((in_fd = open(temp->input, O_RDONLY)) < 0) {
        perror(temp->input);
        return;
      }
   }

   //Grab the number of stages there will be alongside making pipes
   while(temp != NULL) {
      stages += 1;
      //makes pipes between each stage
      if (stages != 1) {
         pipes += 1;
         pipe(&fd[i]);
         i += 2;
      }
      //fprintf(stderr, "stage: %d\n", stages);
      if (temp->next_command != NULL) {
         temp = temp->next_command;
      } else {
         break;
      }
   }

   //create output file redirection for last command
   if (strcmp(temp->output, "original stdout")) {
      //fprintf(stderr, "creating a write only fd for output\n");
      if ((out_fd = open(temp->output, O_WRONLY | O_CREAT | O_TRUNC, 0600)) < 0) {
         perror(temp->output);
         return;
      }
   }

   temp = cmd;

   //Fork stuff from Lab 7 - tryit.c
   int pid = 0;
   int code;
   int status;
   
   //parent loop - forks for each command
   for (i = 0; i < stages; i++) {
      //free any empty argv in Command struct
      for (j = 0; j < 10; j++) {
         if (!strcmp(temp->argv[j], "")) {
            free(temp->argv[j]);
            temp->argv[j] = NULL;
         }
      }
      
      //fork off children
      pid = fork();
      if (pid < 0) {
         perror("fork");
         return;
      }
      //Child process - exec and file redirection
      else if (pid == 0) {
         //input file redirection
         if ((i == 0) && (in_fd != 0)) {
            //fprintf(stderr, "input file descriptor is  %d\n", in_fd);
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
         }
         //output file redirection
         if ((i == stages - 1) && (out_fd != 0)) {
            //fprintf(stderr, "output file descriptor is  %d\n", out_fd);
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
         }

         //pipe redirection (if needed)
         if (pipes != 0) {
            if (i == 0) {
               dup2(fd[1], STDOUT_FILENO);
            } else if (i == pipes) {
               dup2(fd[(pipes-1)*2], STDIN_FILENO);
            } else {
               dup2(fd[(i-1)*2], STDIN_FILENO);
               dup2(fd[(i*2)+1], STDOUT_FILENO);
            }
         }

         //close all file descriptors
         for (i = 0; i < pipes * 2; i++) {
            close(fd[i]);
         }

         //exec stuff is here
         execvp(temp->argv[0], temp->argv);
         perror(temp->argv[0]);
         exit(1);
      }
      temp = temp->next_command;
   }
   //Parent Process
   //close all file descriptors
   for (i = 0; i < pipes * 2; i++) {
      close(fd[i]);
   }
   if (in_fd != 0) {
      close(in_fd);
   }
   if (out_fd != 0) {
      close(out_fd);
   }

   //Wait for each child process
   for (i = 0; i < stages; i++) {
      pid = wait(&status);
      if (WIFEXITED(status)) {
         if ((code = WEXITSTATUS(status)) == 0) {
         } 
      }
      else if (WIFSIGNALED(status)) {
      } else {
         wait(&status);
      }
   }
}

/* Creates a struct Command to be printed to stdout */
int parse_line(char *command_line, struct Command *cmd, int stage_number) {
  char *token;
  int i;
  char cmd_temp[MAX_CMD_LENGTH];
  char cmd_temp_two[MAX_CMD_LENGTH];
  int input_flag = 0;
  int output_flag = 0;
  char output[MAX_OUT_CHAR_LENGTH];
  char input[MAX_IN_CHAR_LENGTH];

  cmd->next_command = NULL;
  
  /* Copies the command_line to a temp */
  /* Needs mutiple versions of the command line for different cases */
  strcpy(cmd_temp_two, command_line);

  /* Initialized output as the default stdout */
  strcpy(cmd->output, "original stdout");

  /* Check if the first argument is a input or output */
  /* Set the respective flag if either input or output is found */
  token = strtok(cmd_temp_two, " ");
  if (!strcmp(token, "<")) {
    input_flag = 1;
  } else if (!strcmp(token, ">")) {
    output_flag = 1;
  }

  /* If input flag is set, set the cmd input */
  /* And shift the pointer of command_line to the next argument */
  if (input_flag == 1) {
    token = strtok(NULL, " ");
    strcpy(cmd->input, token);
    command_line += (2 + strlen(cmd->input));
  }

  /* If output flag is set, set the cmd output */
  /* And shift the pointer of command_line to the next argument */
  if (output_flag == 1) {
    token = strtok(NULL, " ");
    strcpy(cmd->output, token);
    command_line += (2 + strlen(cmd->output));
  }

  /* Copy the current command_line to another temp string */
  strcpy(cmd_temp, command_line);

  /* If there is a pipe, get the current stage */
  token = strtok(cmd_temp, "|");

  /* Copy the current stage to the command_line attribute */
  strcpy(cmd->command_line, token);

  /* Get the next argument */
  token = strtok(command_line, " ");

  /* This argument will be the command and increase argc */
  strcpy(cmd->argv[cmd->argc], token);
  cmd->argc++;

  /* Loop through each stage and repeat */
  while (token != NULL) {
    /* Get the next argument */
    /* If end of command_line, break out of loop */
    token = strtok(NULL, " ");
    if (token == NULL) {
      break;
    }

    /* If argument is input arrow, set input to the next argument */
    if (!strcmp(token, "<")) {
      token = strtok(NULL, " ");
      strcpy(cmd->input, token);
      continue;
      /* If argument is output arrow, set output to the next argument */
    } else if (!strcmp(token, ">")) {
      token = strtok(NULL, " ");
      strcpy(cmd->output, token);
      continue;
      /* If argument is a pipe */
    } else if (!strcmp(token, "|")) {

      /* Set output to correct format */
      /* Set pipe to next stage */
      sprintf(output, "pipe to stage %i", stage_number + 1);
      strcpy(cmd->output, output);
      
      /* Create a new struct and intilize it to the correct values */
      struct Command *next_cmd = (struct Command *)calloc(1, sizeof(struct Command));
      cmd->next_command = (struct Command *)calloc(1, sizeof(struct Command));
      
      /* The next stage will have an input from this current stage */
      sprintf(input, "pipe from stage %i", stage_number);
      strcpy(cmd->input, input);

      /* Reset argc for the next pipe */
      next_cmd->argc = 0;
      for (i = 0; i < PIPE_LIMIT; i++) {
        next_cmd->argv[i] = calloc(1, MAX_CMD_LENGTH);
      }

      /* From this current pipe to the rest of the line */
      /* Is considered the new command_line */
      token = strtok(NULL, "\0");

      /* Use recursision for the next stage */
      cmd->next_command = next_cmd;
      parse_line(token, cmd->next_command, stage_number + 1);
      return 1;
    } else {

      /* If an arugment, save ins argv and increase argc */
      strcpy(cmd->argv[cmd->argc], token);
      cmd->argc++;
    }
  }

  return 1;
}

void free_cmd_struct(struct Command *cmd) {
  int i;
  for (i = 0; i < PIPE_LIMIT; i++){
    free(cmd->argv[i]);
  }
  if (cmd->next_command != NULL) {
    free_cmd_struct(cmd->next_command);
  }
  free(cmd->next_command);
}

int parseline(char *command_line) {
  int i;
  struct Command *cmd = (struct Command *)calloc(1, sizeof(struct Command));

  /* Check the command_line for errors */
  check_command_line(command_line);

  /* Initilize the first struct */
  strcpy(cmd->command_line, command_line);
  strcpy(cmd->input, "original stdin");
  strcpy(cmd->output, "original stdout");
  cmd->argc = 0;
  for (i = 0; i < PIPE_LIMIT; i++) {
    cmd->argv[i] = calloc(1, MAX_CMD_LENGTH);
  }

  /* Parse Line */
  parse_line(command_line, cmd, 0);
  run_command(cmd, 0);
  free_cmd_struct(cmd);
  free(cmd);
  return 0;
}


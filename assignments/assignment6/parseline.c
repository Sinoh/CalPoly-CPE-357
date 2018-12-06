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

/* Call to print the attributes of the struct Command */
/* Print each attribute in the correct format */
void print_command(struct Command *cmd, int stage_number) {
  int i;

  /* Prints the dashes and the the stage number*/
  /* Also prints the appropriate command_line */
  printf("\n--------\n");
  if (stage_number == 0) {
    printf("Stage %i: \"%s\"\n", stage_number, cmd->command_line);
  } else {

    /* Correctly prints the command_line with a space before the command */
    printf("Stage %i: \" %s\"\n", stage_number, cmd->command_line);
  }
  printf("--------\n");

  /* Prints the input, output, argument count, and arguments */
  printf("%10s: %s\n", "input", cmd->input);
  printf("%10s: %s\n", "output", cmd->output);
  printf("%10s: %i\n", "argc", cmd->argc);
  printf("%10s:", "argv");
  for (i = 0; i < cmd->argc; i++) {
    if (i == 0) {
      printf(" \"%s\"", cmd->argv[i]);
    } else {
      printf(",\"%s\"", cmd->argv[i]);
    }
  }
  printf("\n");
  if (cmd->next_command != NULL) {
    print_command(cmd->next_command, stage_number + 1);
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

      /* Print the attributes correctly */
      

      /* Create a new struct and intilize it to the correct values */
      struct Command *next_cmd = (struct Command *)calloc(1, sizeof(struct Command));
      cmd->next_command = (struct Command *)calloc(1, sizeof(struct Command));
      

      /* The next stage will have an input from this current stage */
      sprintf(input, "pipe from stage %i", stage_number);
      strcpy(cmd->input, input);

      /* Reset argc for the next pipe */
      next_cmd->argc = 0;
      for (i = 0; i < MAX_CMD_LENGTH; i++) {
        next_cmd->argv[i] = malloc(MAX_CMD_LENGTH * ONE_BYTE);
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

  /* If the end is reached, print the final piece of the command_line */

  return 1;
}

struct Command *parseline(char *command_line) {
  int i;
  struct Command *cmd = (struct Command *)calloc(1, sizeof(struct Command));

  /* Check the command_line for errors */
  check_command_line(command_line);

  /* Initilize the first struct */
  strcpy(cmd->command_line, command_line);
  strcpy(cmd->input, "original stdin");
  strcpy(cmd->output, "original stdout");
  cmd->argc = 0;
  for (i = 0; i < MAX_CMD_LENGTH; i++) {
    cmd->argv[i] = malloc(MAX_CMD_LENGTH * ONE_BYTE);
  }

  /* Parse Line */
  parse_line(command_line, cmd, 0);
  print_command(cmd, 0);
  return cmd;
}


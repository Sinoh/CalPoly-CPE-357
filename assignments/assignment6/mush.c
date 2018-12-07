#include "mush.h"
#include "parseline.h"

//SIGINT handler
void interrupt_handler() {
   //fprintf(stderr, "Inside interrupt handler\n");
   //this will ignore the kill command the following line
   sigset(SIGINT, SIG_IGN);
   //kills children in same process group
   killpg(getpid(), SIGINT);
   //sets mush to catch SIGINT with this handler
   sigset(SIGINT, interrupt_handler);
}

/* Does the process control (fork, exec, and file redirection) for
   each command*/
void run_command(struct Command *cmd, int stage_number) {
    int stages, pipes, fd[20]; //MAKE 20 A #DEFINE 
    struct Command *temp = NULL;

    stages = pipes = 0;
    temp = cmd;

    //Grab the number stages there will be
    while(temp != NULL) {
        stages += 1;
        pipe(fd);
        //fprintf(stderr, "deeper %d\n", stages);
        temp = temp->next_command;
    }
    fprintf(stderr, "The number of stages is %d\n", stages);

    //Fork stuff from Lab 7 - tryit.c
    pid_t pid = 0;
    //int code;
    //int status;

    if ((pid = fork()) < 0) {
        perror("fork");
        //return -1;
    } else if (pid == 0) {
        //char *args[10] = {cmd->argv[0], NULL};
        if (execvp(cmd->argv[0], cmd->argv) < 0 ) {
            perror(cmd->argv[0]);
        }
    } else {
        
        wait(NULL);
        /*
        wait(&status);
        if (WIFEXITED(status)) {
            if ((code = WEXITSTATUS(status)) == 0) {
                fprintf(stdout, "code: %d\n", code);
                fprintf(stdout, "Process %d succeeded\n", pid);
                //return 0;
            } else {
                fprintf(stdout, "code: %d\n", code);
                fprintf(stdout, "Process %d exited with an error value\n", pid);
                //return -1;
            }
        }
        else if (WIFSIGNALED(status)) {
            fprintf(stdout, "Process %d terminated by signal %d\n", pid, WTERMSIG(status));
        } else {
            wait(&status);
        }  */
    }
  
}

int main(int argc, char **argv) {
    char command_line[MAX_CMD_LENGTH];
    struct sigaction int_act;
    struct Command *cmd = (struct Command *)calloc(1, sizeof(struct Command));
    FILE *fp;
    size_t new_len;

    int_act.sa_handler = interrupt_handler;
    sigaction(SIGINT, &int_act, NULL);

    if (argc == 2) {
        if ((fp = fopen(argv[1], "r")) != NULL) {
            new_len = fread(command_line, sizeof(char), MAX_CMD_LENGTH, fp);
            if (ferror(fp) != 0 ) {
                fputs("Error reading file", stderr);
            } else {
                command_line[new_len++] = '\0';
                parseline(command_line, cmd);
            }
        fclose(fp);
        }
    } else {
        while(1) {
            printf("8-P ");
            //wait for input through stdin
            if (fgets(command_line, MAX_CMD_LENGTH, stdin) == NULL ) {
               //fprintf(stderr, "No Input\n");
               if (feof(stdin)){
                  break;
               }
               fprintf(stderr, "\n");
               continue;
            }

            parseline(command_line, cmd);
            
            if (cmd->argc == 0) {
                printf("%s", "skip\n");
                continue;
            }

            if (!strcmp(cmd->argv[0], "cd")) {
                if (cmd->argc < 2) {
                    fprintf(stderr, "cd: missing argument.\n");
                    continue;
                } else if (cmd->argc > 2) {
                    fprintf(stderr, "cd: too many arguments.\n");
                }

                if(chdir(cmd->argv[1]) != 0) {
                    perror(cmd->argv[1]);
                }
                char cwd[1024]; 
                getcwd(cwd, sizeof(cwd)); 
                printf("Dir: %s\n", cwd);  
                continue;
            

            //check_command_line(command_line);
            //fprintf(stderr, "Going into parseline command\n");
            //exit(1);
            
            } else {
                run_command(cmd, 0);
            }
        }
    }
    printf("\n");
    return 0;
}

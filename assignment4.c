#include "assignment4.h"


/*
    commandLine function adapted from sample_parser.c in Programming Assignment 4: SMALLSH
    https://canvas.oregonstate.edu/courses/1987883/assignments/9864854?module_item_id=25049863
    Accessed 2/15/2025
*/
struct commandLine *parse_input() {
    char input[INPUT_LENGTH];
    struct commandLine* currentCommand = (struct commandLine *) calloc(1, sizeof(struct commandLine));

    // Get input
    printf(": ");
    fflush(stdout);
    fgets(input, INPUT_LENGTH, stdin);

    // Set files to NULL to start
    currentCommand->output_file = NULL;
    currentCommand->input_file = NULL;

    // Tokenize the input
    char* token = strtok(input, " \n");
    
    // If line is blank, ignore
    if(!token) {
        return currentCommand = NULL;
    }

    // If first token = #, ignore the command
    char* firstChar = strndup(token, 1);
    if(!strcmp(firstChar, "#")) {
        return currentCommand = NULL;
    }

    while(token) {
        if(!strcmp(token,"<")){
            currentCommand->input_file = strdup(strtok(NULL," \n"));
        } else if(!strcmp(token,">")) {
            currentCommand->output_file = strdup(strtok(NULL," \n"));
        } else if(!strcmp(token,"&")) {
            currentCommand->is_bg = true;
        } else {
            currentCommand->argv[currentCommand->argc++] = strdup(token);
        }

        token = strtok(NULL," \n");
    }
    return currentCommand;
}


/*
Handles cd command
*/
void cdCommand(struct commandLine* command) {
    // Get current working directory
    char currentWorkingDir[INPUT_LENGTH];
    getcwd(currentWorkingDir, (INPUT_LENGTH * sizeof(char)));

    // Get HOME env variable name
    char* home = getenv("HOME");
    
    // If cd has an argument, change to that directory; else change to directory in HOME env variable
    int status;
    if(command->argc > 1) {
        // Change directory
        chdir(command->argv[1]);
        // Set environment PWD
        setenv("PWD", command->argv[1], 1);
    } else {
        // Change directory
        chdir(home);
        // Set environment PWD
        setenv("PWD", home, 1);
    }
}


// Prints out either the exit status or the terminating signal of the last foreground process ran by your shell
void statusCommand(int exitNum, int signalNum, bool termBySignal) {
    if(!termBySignal) {
        // If printing exit status
        printf("exit value %d\n", exitNum);
        fflush(stdout);
    } else {
        // If printing terminating signal
        printf("terminated by signal %d\n", signalNum);
        fflush(stdout);
    }
}




/*
    Starts a new process using a non-built-in command
    Adapted from CS374 Exploration: Process API – Creating and Terminating Processes
    https://canvas.oregonstate.edu/courses/1987883/pages/exploration-process-api-creating-and-terminating-processes?module_item_id=24956218
    Accessed 2/15/2025
    and
    Exploration: Process API - Executing a New Program
    https://canvas.oregonstate.edu/courses/1987883/pages/exploration-process-api-executing-a-new-program?module_item_id=24956220
    Accessed 2/15/2025
    and
    Exploration: Processes and I/O
    https://canvas.oregonstate.edu/courses/1987883/pages/exploration-processes-and-i-slash-o?module_item_id=24956228
    Accessed 2/17/2025
    and
    Exploration: Signal Handling API
    https://canvas.oregonstate.edu/courses/1987883/pages/exploration-signal-handling-api?module_item_id=24956227
    Accessed 2/20/2025
*/
void newProcess(struct commandLine* command, int* exitStatus, bool* termBySignal, int* signalNum, int backgroundPids[]) {
    pid_t spawnPid = -5;
    int childStatus;
    static int pidCount = 0;


    // Ignore SIGINT in parent and child
    struct sigaction SIGINT_action = {0};
    SIGINT_action.sa_handler = SIG_IGN;
    sigaction(SIGINT, &SIGINT_action, NULL);


    // If fork is successful, child's spawnid = 0 and parent's spawnid = child's pid
    spawnPid = fork();
    char output[] = ">";
    char input[] = "<";

    switch (spawnPid){
        case -1:
            // If fork failed
            perror("fork() failed!");
            exit(1);
            break;
        case 0:
            // spawnpid is 0 in the child

            // If child process is in foreground, restore SIGINT functionality
            if(!command->is_bg) {
                SIGINT_action.sa_handler = SIG_DFL;
                sigaction(SIGINT, &SIGINT_action, NULL); 
            }

            // If argv has an output file, then redirect output
            if(command->output_file) {
                // Open the file
                int fileDesc = open(command->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0640);
                if (fileDesc == -1) {
                    *exitStatus = 1;
                    perror(command->output_file);
                    exit(1);
                }

                // Redirect stdout to file
                int result = dup2(fileDesc, 1);
                if (result == -1) { 
                    *exitStatus = 2;
                    perror("source dup2()"); 
                    exit(2); 
                }
            } else if(command->is_bg) {
                // If there is no output redirection and command is in the background
                // Redirect stdout to /dev/null
                // Open the file
                int fileDesc = open("/dev/null", O_WRONLY | O_CREAT | O_TRUNC, 0640);
                if (fileDesc == -1) {
                    *exitStatus = 1;
                    perror(command->output_file);
                    exit(1);
                }

                // Redirect stdout to file
                int result = dup2(fileDesc, 1);
                if (result == -1) { 
                    *exitStatus = 2;
                    perror("source dup2()"); 
                    exit(2); 
                }
            }

            // If argv has an input file, then redirect output
            if(command->input_file) {
                // Open the file
                int fileDesc = open(command->input_file, O_RDONLY, 0640);
                if (fileDesc == -1) {
                  *exitStatus = 1;
                  perror(command->input_file);
                  exit(1);
                }

                // Redirect stdin to file
                int result = dup2(fileDesc, 0);
                if (result == -1) { 
                    *exitStatus = 2;
                    perror("source dup2()"); 
                    exit(2); 
                }
            } else if(command->is_bg) {
                // If there is no input redirection and command is in the background
                // Redirect stdout to /dev/null
                // Open the file
                int fileDesc = open("/dev/null", O_RDONLY, 0640);
                if (fileDesc == -1) {
                    *exitStatus = 1;
                    perror(command->output_file);
                    exit(1);
                }

                // Redirect stdout to file
                int result = dup2(fileDesc, 0);
                if (result == -1) { 
                    *exitStatus = 2;
                    perror("source dup2()"); 
                    exit(2); 
                }
            }
            
            // Run the new program in the child
            execvp(command->argv[0], command->argv);

            // If there is an error
            *exitStatus = 1;
            perror(command->argv[0]);
            exit(1);
            break;
        default:
            // spawnpid is the pid of the child
            // Wait for the child process to finish in the foreground
            if(!command->is_bg) {
                spawnPid = waitpid(spawnPid, &childStatus, 0);
                // If exited normally, set exitStatus; else, set signalNum
                if(WIFEXITED(childStatus)) {
                    *termBySignal = false;
                    *exitStatus = WEXITSTATUS(childStatus);
                } else {
                    *termBySignal = true;
                    *signalNum = WTERMSIG(childStatus);
                }
            } else {
                // If child process is started in the background
                printf("background pid is %d\n", spawnPid);
                fflush(stdout);

                // Add background pid to array
                pidCount++;
                backgroundPids[pidCount] = spawnPid;
                
                waitpid(-1, &childStatus, WNOHANG);
            }


            break;
    }
    
}


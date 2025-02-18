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

    // Tokenize the input
    char* token = strtok(input, " \n");
    
    // If line is blank, ignore
    if(!token) {
        return currentCommand = NULL;
    }

    // If first token = #, ignore the command
    char* firstChar = strdup(&token[0]);
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
        chdir(command->argv[1]);
    } else {
        chdir(home);
    }
}


// Prints out either the exit status or the terminating signal of the last foreground process ran by your shell
void statusCommand(int exitOrSignalNum, bool termBySignal) {
    if(!termBySignal) {
        // If printing exit status
        printf("exit value %d\n", exitOrSignalNum);
        fflush(stdout);
    } else {
        // If printing terminating signal
        printf("terminated by signal %d", exitOrSignalNum);
        fflush(stdout);
    }
}




/*
    Starts a new process using a non-built-in command
    Adapted from CS374 Exploration: Process API – Creating and Terminating Processes
    https://canvas.oregonstate.edu/courses/1987883/pages/exploration-process-api-creating-and-terminating-processes?module_item_id=24956218
    and
    Exploration: Process API - Executing a New Program
    https://canvas.oregonstate.edu/courses/1987883/pages/exploration-process-api-executing-a-new-program?module_item_id=24956220
    Accessed 2/15/2025
*/
void newProcess(struct commandLine* command, int* exitStatus) {
    pid_t spawnPid = -5;
    int childStatus;

    // If fork is successful, child's spawnid = 0 and parent's spawnid = child's pid
    spawnPid = fork();

    switch (spawnPid){
        case -1:
            // If fork failed
            perror("fork() failed!");
            break;
        case 0:
            // spawnpid is 0 in the child
            // Run the new program in the child
            execvp(command->argv[0], command->argv);
            // If there is an error
            perror("badfile: no such file or directory");
            *exitStatus = 1;
            exit(1);
            break;
        default:
            // spawnpid is the pid of the child
            // Wait for the child process to finish
            spawnPid = waitpid(spawnPid, &childStatus, 0);
            *exitStatus = 0;
            exit(0);
            break;
    }
    
}
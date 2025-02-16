#include "assignment4.h"


/*
    command_line function adapted from sample_parser.c in Programming Assignment 4: SMALLSH
    https://canvas.oregonstate.edu/courses/1987883/assignments/9864854?module_item_id=25049863
    Accessed 2/15/2025
*/
struct command_line *parse_input() {
    char input[INPUT_LENGTH];
    struct command_line* currentCommand = (struct command_line *) calloc(1, sizeof(struct command_line));

    // Set all argv[] values to NULL to start
    for(int i = 0; i < MAX_ARGS+1; i++) {
        currentCommand->argv[i] = NULL;
    }

    // Get input
    printf(": ");
    fflush(stdout);
    fgets(input, INPUT_LENGTH, stdin);

    // Tokenize the input
    char* token = strtok(input, " \n");
    
    // If line is blank, ignore
    if(!token) {
        return 0;
    }

    // If first token = #, ignore the command
    char* firstChar = strdup(&token[0]);
    if(!strcmp(firstChar, "#")) {
        return 0;
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




int cdCommand(struct command_line* command) {
    // Get current working directory
    char currentWorkingDir[INPUT_LENGTH];
    getcwd(currentWorkingDir, (INPUT_LENGTH * sizeof(char)));

    // Get HOME env variable name
    char* home = getenv("HOME");
    
    // If cd has an argument, change to that directory; else change to directory in HOME env variable
    int status;
    if(command->argv[1]) {
        status = chdir(command->argv[1]);
    } else {
        status = chdir(home);
    }

    return status;
}



/*
    Adapted from CS374 Exploration: Process API – Creating and Terminating Processes
    https://canvas.oregonstate.edu/courses/1987883/pages/exploration-process-api-creating-and-terminating-processes?module_item_id=24956218
    Accessed 2/15/2025
*/
// int newProcess() {
//     pid_t spawnpid = -5;
//     int intVal = 10;
//     // If fork is successful, child's spawnid = 0 and parent's spawnid = child's pid
//     spawnpid = fork();
//     switch (spawnpid){
//         case -1:
//             perror("fork() failed!");
//             exit(1);
//             break;
//         case 0:
//             // spawnpid is 0 in the child
//             intVal = intVal + 1;
//             printf("I am the child! intVal = %d\n", intVal);
//             break;
//         default:
//             // spawnpid is the pid of the child
//             intVal = intVal - 1;
//             printf("I am the parent! intVal = %d\n", intVal);
//             break;
//    }
//     printf("This statement will be executed by both of us!\n");

// }
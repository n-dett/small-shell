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
        return;
    }

    // If first token = #, ignore the command
    char* firstChar = strdup(&token[0]);
    if(!strcmp(firstChar, "#")) {
        return;
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


#include "assignment4.h"

int main() {
    struct command_line *currentCommand;
    int exitStatus = 0;

    // Parse the user input
    while(true) {
        currentCommand = parse_input();
    }

    // If current command is "exit", then exit the process
    if(!strcmp(currentCommand->argv, "exit")) {
        // kill background processes first?
        exit(exitStatus);
    }

    // If current command is "cd"
    if(!strcmp(currentCommand->argv, "cd")) {
        exitStatus = cdCommand(currentCommand);
    }

    
    return EXIT_SUCCESS;
}

// If..then call function, return exit status
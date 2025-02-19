#include "assignment4.h"

int main() {
    struct commandLine *currentCommand;
    int zero = 0;
    int* exitStatus = &zero;
    bool termBySignal = false;

    // Parse the user input
    while(true) {
        currentCommand = parse_input();

        if(!currentCommand) {
            // If currentCommand is NULL
            continue;
        } else if(!strcmp(currentCommand->argv[0], "exit")) {
            // If current command is "exit", then exit the process
            exit(*exitStatus);
        } else if(!strncmp(currentCommand->argv[0], "cd", 2)) {
            // If current command is "cd"
            cdCommand(currentCommand);
        } else if(!strncmp(currentCommand->argv[0], "status", 6)) {
            statusCommand(*exitStatus, termBySignal);
        } else {
            // If not a built-in command, start a new process
            newProcess(currentCommand, exitStatus);
        } 
    }
    
    return EXIT_SUCCESS;
}

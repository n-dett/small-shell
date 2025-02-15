#include "assignment4.h"

int main() {
    struct command_line *curr_command;
    int status = 0;
    while(true) {
        curr_command = parse_input();
    }

    // If current command is "exit", then exit the process
    if(!strcmp(curr_command->argv, "Exit")) {
        exit(status);
    }

    
    return EXIT_SUCCESS;
}

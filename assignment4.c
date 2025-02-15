#include "assignment4.h"


/*
    Function adapted from sample_parser.c in Programming Assignment 4: SMALLSH
    https://canvas.oregonstate.edu/courses/1987883/assignments/9864854?module_item_id=25049863
    Accessed 2/15/2025
*/
struct command_line *parse_input() {
    char input[INPUT_LENGTH];
    struct command_line* curr_command = (struct command_line *) calloc(1, sizeof(struct command_line));

    // Get input
    printf(": ");
    fflush(stdout);
    fgets(input, INPUT_LENGTH, stdin);

    // Tokenize the input
    char* token = strtok(input, " \n");
    
    // If first token = #, ignore the command
    char* firstChar = strdup(&token[0]);
    if(!strcmp(firstChar, "#")) {
        exit(0);
    }

    while(token) {
        if(!strcmp(token,"<")){
            curr_command->input_file = strdup(strtok(NULL," \n"));
        } else if(!strcmp(token,">")) {
            curr_command->output_file = strdup(strtok(NULL," \n"));
        } else if(!strcmp(token,"&")) {
            curr_command->is_bg = true;
        } else {
            curr_command->argv[curr_command->argc++] = strdup(token);
        }

        token = strtok(NULL," \n");
    }
    return curr_command;
}

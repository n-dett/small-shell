#ifndef ASSIGNMENT4
#define ASSIGNMENT4

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#define INPUT_LENGTH 2048
#define MAX_ARGS 512

/*
    Struct copied from sample_parser.c in Programming Assignment 4: SMALLSH
    https://canvas.oregonstate.edu/courses/1987883/assignments/9864854?module_item_id=25049863
    Accessed 2/15/2025
*/
struct command_line {
    char* argv[MAX_ARGS + 1];
    int argc;
    char* input_file;
    char* output_file;
    bool is_bg;
};

struct command_line *parse_input();
//int newProcess();
int cdCommand(struct command_line* command);






#endif

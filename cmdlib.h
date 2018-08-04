/**
 * Header file for 'cmdlib.c'
 * 
 * @author Stanley Fuller
 */

#include <stdio.h>
#include <stdbool.h>

struct argument {
    // A string containing the argument code given, for example: "-a" would be
    // stored as "a"
    char *pretext;

    // The text that followed the argument
    char *content;
};

int get_arguments(int argc, char* argv[], struct argument* args);
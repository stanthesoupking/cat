#include "cmdlib.h"

/**
 * Returns all arguments supplied in the form of the argument struct
 * 
 * @param argc total number of arguments given
 * @param argv array of argument strings
 * @param args array of arguments to place argument values into
 * 
 * @return number of arguments found
 */
int get_arguments(int argc, char* argv[], struct argument* args) {
    // Current index in arg array
    int args_i = -1;

    // If set to true then the next string should be option content
    bool get_content = false;

    // Loop through arguments
    for(int i = 1; i < argc; i++) {
        char *argument = argv[i];
        if(argument[0] == '-') { // If argument is a pretext, eg: '-option'
            // Assign pretext to new argument entry
            args[++args_i].pretext = argument;

            // If content is given in the next argument then assign it to this
            // entry
            get_content = true;
        }
        else {
            // If the argument content is not part of an option then create new
            // argument entry without a pretext
            if(!get_content)
                args_i++;

            // Add content to argument entry
            args[args_i].content = argument;
            get_content = false;
        }
    }

    return args_i+1;
}
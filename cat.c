/**
 * Concatenates and displays given files. File paths are inputted through use
 * of command arguments.
 * 
 * The command layout is as follows:
 * cat <input-file1>, <input-file2>, <...> [-o <output-file>]
 * 
 * @author Stanley Fuller 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cmdlib.h"

// Maximum character length for the line buffer
#define BUFFER_SIZE 255

// Maximum number of files allowed
#define MAX_FILES 255

// Argument for displaying help screen
#define PRETEXT_HELP "--help"

// Argument for displaying help screen
#define PRETEXT_OUTPUT "-o"

// Method declarations:
bool display_file(char *filepath);
void display_error(char *text);
void display_help();

int current_file = 0;

bool output_to_file = false;
FILE *output_file;

int main(int argc, char *argv[]) {
    struct argument *args = malloc(argc * sizeof(struct argument));
    int argument_count = get_arguments(argc, argv, args);

    int filepath_i = 0;
    char *filepaths[MAX_FILES];

    // Loop through commands given and process them
    for(int i = 0; i < argument_count; i++) {
        // If no pretext is given, assume argument is a filepath
        if(args[i].pretext == NULL) {
            // Check to see if file limit has been reached
            if(filepath_i + 1 > MAX_FILES) {
                display_error("file limit exceeded");
                return 0;
            }

            // Add filepath to the end of the filepath array
            filepaths[filepath_i++] = args[i].content;
        }
        else if (!strcmp(args[i].pretext, PRETEXT_HELP)) {
            display_help();
            return 0;
        }
        else if(!strcmp(args[i].pretext, PRETEXT_OUTPUT)) {
            if(args[i].content != NULL) {
                output_file = fopen(args[i].content, "w");

                if(output_file == NULL) {
                    display_error("couldn't open/create output file");
                }

                output_to_file = true;
            }
            else {
                display_error("no output file specified");
                return 0;
            }
        }
        else {
            // Unknown command entered, display error message and halt
            char message[255];
            strcpy(message, "unknown command '");
            strcat(message, args[i].pretext);
            strcat(message, "' entered");
            display_error(message);
            return 0;
        }
    }

    // Display error if no filepaths were provided
    if(filepath_i == 0) {
        display_error("no filepaths provided");
        return 0;
    }

    // Display files
    for(int i = 0; i < filepath_i; i++) {
        if(!display_file(filepaths[i])) {
            // If file couldn't be displayed
            char message[255];
            strcpy(message, "file '");
            strcat(message, filepaths[i]);
            strcat(message, "' couldn't be displayed");
            display_error(message);
        }
    }

    return 0;
}

/**
 * Reads and displays the file at the given path
 * 
 * @param filepath the path of the file to read
 * 
 * @return true if the file displayed correctly, false if an error occurred
 *      while reading the file.
 */
bool display_file(char *filepath) {
    FILE *f;
    
    // Attempt to open file
    if(!(f = fopen(filepath, "r"))) {
        return false;   // File open failed, return false
    }

    // Buffer for temporarily storing file line content
    char buffer[BUFFER_SIZE];

    // Loop through each line of the file
    int i = 0;
    while(fgets(buffer, 255, f)) {
        i++;
        if(output_to_file) {
            fprintf(output_file, "%s", buffer);
        }
        else {
            printf("%s", buffer);
        }
    }

    if(!output_to_file) {
        // Add line break to the final line of text
        printf("\n");
    }

    // Close the file
    fclose(f);

    return true;
}

/**
 * Displays an error with the given text
 * 
 * @param text text to put inside the error message
 */
void display_error(char *text) {
    printf("\033[31mERROR: %s\033[0m\n", text);
}

/**
 * Displays help text for the program
 */
void display_help() {
    printf("Concatenate and display given files. File paths are inputted through use\n");
    printf("of command arguments.\n");
    printf("The command layout is as follows:\n");
    printf("cat <input-file1>, <input-file2>, <...> [-o <output-file>]\n");
}
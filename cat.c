/**
 * Concatenates and displays given files. File paths are inputted through use
 * of command arguments.
 * 
 * The command layout is as follows:
 * cat <input-file1>, <input-file2>, <...> [-o <output-file>] [--help] [--catify]
 * 
 * @author Stanley Fuller 
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cmdlib.h"
#include <wchar.h>
#include <locale.h>

// Maximum character length for the line buffer
#define BUFFER_SIZE 255

// Maximum number of files allowed
#define MAX_FILES 255

// What chance is there to insert a cat per character in 'catify' mode (1/<value>)
#define CAT_FACTOR 10

// Argument for displaying help screen
#define PRETEXT_HELP "--help"

// Argument for displaying help screen
#define PRETEXT_OUTPUT "-o"

// Argument for inserting cats into output
#define PRETEXT_CATIFY "--catify"

// Cat emoji
const wchar_t cat_emoji = 0x1F431;

// True if the file output argument was set
bool output_to_file = false;

// File to output results to
FILE *output_file;

// True if the output will be 'catified'
bool do_catify = false;

// Method declarations:
bool display_file(char *filepath);
void display_error(char *text);
void display_help();
void catify_output(char *text);
void fcatify_output(FILE *f, char *text);

int main(int argc, char *argv[]) {
    // Initialise random number generator for the 'catify' function
    srand(time(NULL));

    // Set locale to UTF-8 to enable emoji support
    setlocale(LC_ALL, "en_US.utf8");

    struct argument *args = malloc(argc * sizeof(struct argument));
    int argument_count = get_arguments(argc, argv, args);

    int filepath_i = 0; // Filepath iterator
    char *filepaths[MAX_FILES]; // Filepath array

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
        else if(!strcmp(args[i].pretext, PRETEXT_CATIFY)) {
            do_catify = true;
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
            if(do_catify) {
                fcatify_output(output_file, buffer);
            }
            else {
                fprintf(output_file, "%s", buffer);
            }
        }
        else {
            if(do_catify) {
                catify_output(buffer);
            }
            else {
                printf("%s", buffer);
            }
        }
    }

    if(!output_to_file) {
        // Add line break to the final line of text
        printf("\n");
    }
    else {
        fclose(output_file);
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
    printf("cat <input-file1>, <input-file2>, <...> [-o <output-file>] [--help] [--catify]\n");
}

/**
 * Displays a string and inserts cat emojis at random points
 * 
 * @param text the text to display w/ inserted cat emojis
 */
void catify_output(char *text) {
    int i = 0;
    char c;

    // Loop through all characters in text
    while((c = text[i++]) != '\0') {
        if(!(rand() % CAT_FACTOR)) {
            printf("%lc", cat_emoji);
        }
        printf("%c", c);
    }
}

/**
 * Outputs a string to a given file and inserts cat emojis at random points
 * 
 * @param text the text to output to file w/ inserted cat emojis
 */
void fcatify_output(FILE *f, char *text) {
    int i = 0;
    char c;

    // Loop through all characters in text
    while((c = text[i++]) != '\0') {
        if(!(rand() % CAT_FACTOR)) {
            fprintf(f, "%lc", cat_emoji);
        }
        fprintf(f, "%c", c);
    }
}
/* Utility functions for the NSP program
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 21/11/2018
   Edited: 17/02/2019 */

#ifndef NSP_UTIL_HPP
#define NSP_UTIL_HPP

#include <getopt.h>
#include <fstream>

#include "../../classes/include/NSP.hpp"

#define VERSION "0.1a"
#define NOK 0
#define OK 1

#define ARGSSTR "hvk:m:"
#define HELPSTR "\
            Nurse scheduling algorithm\n\n\
Usage: nsp instace-file case-file output-file <options>\n\
    Options:\n\
        -h --help:    Prints this help message\n\
        -v --verbose: Prints verbose execution information\n\
           --version: Prints information about this program's version\n\n\
Questions to cristoferoswald@gmail.com"
#define VERSIONSTR "Nurse scheduling algorithm, version: " VERSION
#define INVALIDSTR "Invalid usage, \"-h\" for help."
#define VERBOSESTR "Verbose mode is set\n"
#define OUTPUTHEADERSTR "\
    Nurse schedaling problem algorithm output file\n\n"

extern struct args_t{
    char *instance_file;
    char *case_file;
    char *output_file;
    bool verbose;
    bool show_help;
    bool show_version;
    int k;
    int max_depth;
} args;

extern const struct option long_options[];
extern NSP problem;
extern std::ofstream *output_file;
extern int end_type;

/**
 * Reads the arguments passed, setting the values in the arguments structure
 * @param argc Argument count
 * @param argv Argument list
 */
void readArgs(int argc, char** argv);

/**
 * Prints help message and ends the program
 */
void showHelp();

/**
 * Prints the version message and exits
 */
void showVersion();

/**
 * Informs the user that verbose mode is on
 */
void showVerbose();

/**
 * Reads the problem files (instance-file and case-file) and creates the needed structures and the output file
 */
void readProblem();

void cantOpen(char *file);
void instanceCaseMismatch(int i, int c);

/**
 * Exits the program, freeing what is necessary.
 */
void endProgram();

#endif /* NSP_UTIL_HPP */

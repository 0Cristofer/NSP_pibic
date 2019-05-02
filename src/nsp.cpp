/* Nurse scheduling problem main file.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 21/11/2018
   Edited: 17/02/2019 */

#include <iostream>

#include "util/include/util.hpp"

NSP problem;
std::ofstream *output_file;
int end_type = OK;

int main(int argc, char **argv) {
    readArgs(argc, argv);

    if(args.show_help){
        showHelp();
    }
    if(args.show_version){
        showVersion();
    }

    if(args.verbose) showVerbose();

    std::cout << R"(Starting...)" << std::endl << std::endl;

    readProblem();

    problem.solve();

    endProgram();
}

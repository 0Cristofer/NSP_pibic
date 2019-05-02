/* Utility functions for the NSP program
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 21/11/2018
   Edited: 17/02/2019 */

#include <getopt.h>
#include <iostream>
#include <iomanip>
#include <ctime>

#include "include/util.hpp"

args_t args;

void unknwonOpt(char** argv);
void invalidUsage();

// "help" and "verbose" have a short option, so they need to be set in the switch condition
const struct option long_options[] = {
        {"help",    no_argument, nullptr, 'h'},
        {"verbose", no_argument, nullptr, 'v'},
        {"version", no_argument, (int*)(&(args.show_version)), true},
        {nullptr, 0, nullptr, 0}
};


/**
 * Initializate the arguments structure, setting all parameters and options to default values
 */
void initArgs () {
    args.instance_file = nullptr;
    args.case_file = nullptr;
    args.output_file = nullptr;

    args.show_help = false;
    args.show_version = false;
    args.verbose = false;
}

void readArgs(int argc, char** argv) {
    int opt;

    opterr = 0;
    initArgs();

    while ((opt = getopt_long(argc, argv, ARGSSTR, long_options, nullptr)) != -1) {
        switch (opt){
            case 'h':
                args.show_help = true;
                break;
            case 'v':
                args.verbose = true;
                break;
            case '?':
                unknwonOpt(argv);
            default:
                break;
        }
    }

    if(((argc - optind) < 3) && ((!args.show_version) && (!args.show_help))){
        invalidUsage();
    }
    else if((argc - optind) > 3){
        invalidUsage();
    }
    else{
        if(args.show_help || args.show_version){
            return;
        }

        args.instance_file = argv[optind];
        args.case_file = argv[optind+1];
        args.output_file = argv[optind+2];
    }
}

/**
 * Prints the default invalid usage error and exits.
 */
void invalidUsage(){
    end_type = NOK;
    std::cerr << INVALIDSTR;
    exit(0);
}

/**
 * Prints the default error message, informing the wrong option and then exits the program
 * @param argv The program's arguments
 */
void unknwonOpt(char** argv){
    end_type = NOK;
    std::cerr << "Unknown option: \"" << argv[optind-1] << R"(", use "-h" for help. Aborting.)" << std::endl;
    exit(0);
}

void showHelp(){
    std::cout << HELPSTR;
    exit(0);
}

void showVersion(){
    std::cout << VERSIONSTR;
    exit(0);
}

void showVerbose(){
    std::cout << VERBOSESTR;
}

void readProblem() {
    int in, total_shifts;
    std::ifstream instance_file;
    std::ifstream case_file;
    std::time_t t;
    std::tm *now;
    std::string input1, input2;

    output_file = new std::ofstream();

    if(args.verbose){
        std::cout << "Reading problem files..." << std::endl;
        std::cout << R"(    Instance file: )" << args.instance_file << std::endl;
        std::cout << R"(    Case file: )" << args.case_file << std::endl;
        std::cout << R"(    Output file: )" << args.output_file << std::endl;
    }

    instance_file.open(args.instance_file);
    if(!instance_file.is_open()) cantOpen(args.instance_file);

    case_file.open(args.case_file);
    if(!case_file.is_open()) cantOpen(args.case_file);

    output_file->open(args.output_file);
    if(!output_file->is_open()) cantOpen(args.output_file);

    instance_file >> input1; // Number of nurses
    problem.n_nurses = std::stoi(input1);

    instance_file >> input1; // Number of days
    problem.n_days = std::stoi(input1);

    instance_file >> input1; // Number of shifts
    problem.n_shifts = std::stoi(input1);

    // Instance file reading

    // Reads shift/day minimum coverage
    for(int i = 0; i < problem.n_days; i++){
        total_shifts = 0;

        problem.days.push_back(new Day(i));
        for(int j = 0; j < problem.n_shifts; j++){
            instance_file >> input1;
            problem.days[i]->nurses_by_shift.push_back(std::stoi(input1));

            for(int k = 0; k < problem.days[i]->nurses_by_shift[j]; k++) {
                problem.days[i]->shifts.push_back(new Shift(j, i));
                total_shifts = total_shifts + 1;
            }
        }

        for(int j = 0; j < (problem.n_nurses - total_shifts); j++)
            problem.days[i]->shifts.push_back(new Shift(problem.n_shifts-1, i));
    }

    // Reads nurses preferences
    for(int i = 0; i < problem.n_nurses; i++){
        problem.nurses.push_back(new Nurse(i));

        for(int j = 0; j < problem.n_shifts; j++) {
            problem.nurses[i]->current_state->number_assignments_shifts.push_back(0);
            problem.nurses[i]->current_state->reached_min_same_assign.push_back(false);
            problem.nurses[i]->current_state->reached_min_assign.push_back(false);
        }

        for(int j = 0; j < problem.n_days; j++){
            for(int k = 0; k < problem.n_shifts; k++){
                instance_file >> input1;
                problem.nurses[i]->preferences.push_back(std::stoi(input1));
            }
        }
    }

    // Case file reading

    case_file >> input1; // Number of days
    in = std::stoi(input1);
    if(problem.n_days != in) instanceCaseMismatch(problem.n_days, in);

    case_file >> input1; // Number of days
    in = std::stoi(input1);
    if(problem.n_shifts != in) instanceCaseMismatch(problem.n_shifts, in);

    case_file >> input1; // Min assignments
    problem.nsp_case.min_asg = std::stoi(input1);
    case_file >> input1; // Max assignments
    problem.nsp_case.max_asg = std::stoi(input1);

    case_file >> input1; // Min consecutive working shifts
    problem.nsp_case.min_cws = std::stoi(input1);
    case_file >> input1; // Max consecutive working shifts
    problem.nsp_case.max_cws = std::stoi(input1);

    // Reads min/max consecutive same working shifts and min/max assignments per shift
    for(int i = 0; i < problem.n_shifts; i++){
        case_file >> input1;
        case_file >> input2;
        problem.nsp_case.con_same_shift.emplace_back(std::stoi(input1), std::stoi(input2));

        case_file >> input1;
        case_file >> input2;
        problem.nsp_case.assg_shift.emplace_back(std::stoi(input1), std::stoi(input2));
    }

    std::cout << "Reading files done." << std::endl << std::endl;
    if(args.verbose){
        problem.printProblem();
    }

    t = std::time(nullptr); // Get time now
    now = std::localtime(&t);

    *output_file << OUTPUTHEADERSTR;
    *output_file << std::setfill('0');
    *output_file << "Run start at " << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' << now->tm_mday
                 << ", " << std::setw(2) << now->tm_hour << ":" << std::setw(2) << now->tm_min << ":"
                 << std::setw(2) << now->tm_sec << std::endl << std::endl;
    *output_file << "Instance file: " << args.instance_file << std::endl;
    *output_file << "Case file: " << args.case_file << std::endl << std::endl;

    instance_file.close();
    case_file.close();
}

void cantOpen(char *file){
    end_type = NOK;
    std::cerr << "Couldn't open file: " << file << ", aborting." << std::endl;
    endProgram();
}

void instanceCaseMismatch(int i, int c){
    end_type = NOK;
    std::cerr << "Instace/case files mismatch, number of days/shifts is diferent" << std::endl;
    std::cerr << "Instance: " << i << ", Case: " << c << std::endl;
    endProgram();
}

void endProgram(){
    std::time_t t;
    std::tm *now;

    t = std::time(nullptr);
    now = std::localtime(&t);

    *output_file << "Run end at " << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday
                 << ", " << std::setw(2) <<  now->tm_hour << ":" << std::setw(2) << now->tm_min
                 << ":" << std::setw(2) << now->tm_sec << std::endl << std::endl;

    if (!end_type){
        output_file->close();
        delete output_file;

    }
    else {
        *output_file << "Final assignments:" << std::endl;
        for (int i = 0; i < problem.n_nurses; i++) {
            for(Day* day: problem.days){
                if(day->shifts[i]->is_spare)
                    *output_file << day->shifts[i]->nurse->number << "(" << day->shifts[i]->type << "S)" << " ";
                else
                    *output_file << day->shifts[i]->nurse->number << "(" << day->shifts[i]->type << ")" << " ";

                if(day->shifts[i]->nurse->number < 10) *output_file << " "; // Extra space to compensate the extra digit
            }
            *output_file << std::endl;
        }
    }

    exit(0);
}

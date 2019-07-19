/* Solution finder algorithm for APC.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 10/07/2019
   Edited: 16/07/2019 */

#include <stdlib.h>
#include <cstdio>
#include <map>

#include "include/apcsolution.hpp"

int n;
int *marked_cols;
int num_marked = 0;

int **cost_matrix;
int k;
int k_total;

int *line_map;

void init(int num){
    n = num;
    marked_cols = (int *)malloc(sizeof(int) * n);
    line_map = (int *)malloc(sizeof(int) * n);

    for(int i = 0; i < n; i++){
        marked_cols[i] = 0;
    }
}

std::vector<std::vector<int> *> *getSolution(int line){
    auto *solutions = new std::vector<std::vector<int> *>();
    std::vector<std::vector<int> *> *new_solutions;
    std::vector<int> * new_solution;

    if(line >= n) return solutions;

    for(int j = 0; (j < n) && (k_total < k); j++){
        if(marked_cols[j]) continue;

        if(cost_matrix[line_map[line]][j] == 0){

            marked_cols[j] = 1;
            num_marked = num_marked + 1;

            new_solutions = getSolution(line+1);

            if(new_solutions->empty()) {
                if((num_marked) == n) {
                    new_solution = new std::vector<int>;
                    new_solution->push_back(j);
                    solutions->push_back(new_solution);
                    k_total = k_total + 1;
                }
            }
            else {
                for (std::vector<int> *new_line : *new_solutions) {
                    new_solution = new std::vector<int>;
                    new_solution->push_back(j);
                    new_solution->insert(new_solution->end(), new_line->begin(), new_line->end());
                    solutions->push_back(new_solution);

                    delete(new_line);
                }
            }

            delete(new_solutions);
            num_marked = num_marked - 1;
            marked_cols[j] = 0;
        }
    }

    return solutions;
}

void createMap(){
    int n0s;
    std::map<std::pair<int, int>, int> n0s_map;

    for(int i = 0; i < n; i++){
        n0s = 0;
        for(int j = 0; j < n; j++){
            n0s = n0s + (cost_matrix[i][j] == 0);
        }
        n0s_map[std::pair<int, int>(n0s, i)] = i;

    }

    n0s = 0;
    for(std::pair<std::pair<int, int>, int> zeros : n0s_map){
        line_map[n0s] = zeros.second;
        n0s = n0s + 1;
    }
}

std::vector<std::vector<int> *> *getSolutions(int **a, int num_sol){
    std::vector<std::vector<int> *> *solutions;
    std::vector<std::vector<int> *> *new_solutions;

    cost_matrix = a;
    k = num_sol;
    k_total = 0;

    createMap();

    solutions = getSolution(0);

    new_solutions = new std::vector<std::vector<int> *>();

    for(int cur_sol = 0; cur_sol < solutions->size(); cur_sol++) {
        new_solutions->push_back(new std::vector<int>(n));
        for (int i = 0; i < n; i++) {

            (*(*new_solutions)[cur_sol])[line_map[i]] = (*(*solutions)[cur_sol])[i];
        }

        delete((*solutions)[cur_sol]);
    }
    delete(solutions);

    return new_solutions;
}

void clear(){
    free(marked_cols);
    free(line_map);
}
/* Solution finder algorithm for APC.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 10/07/2019
   Edited: 16/07/2019 */

#ifndef NSP_APCSOLUTION_HPP
#define NSP_APCSOLUTION_HPP

#include <vector>

/**
 * Initializates the solution getter. Must be called before using the algorithm
 * @param num The size of the problem
 */
void init(int num);

/**
 * Gets the first k possible solutions of a given reduced cost matrix
 * @param a The cost matrix
 * @param k The number of solutions to be found
 * @return The vector containing the solutions. Each element is a vector representing a solution
 */
std::vector<std::vector<int> *> *getSolutions(int **a, int k);

/**
 * Clears variables needed for the algorithm. Must be called at the end
 */
void clear();

#endif //NSP_APCSOLUTION_HPP

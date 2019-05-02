/* Nurse scheduling shift class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 07/02/2019
   Edited: 17/02/2019 */


#ifndef NSP_SHIFT_HPP
#define NSP_SHIFT_HPP

#include <vector>

#include "Nurse.hpp"

class Shift {
public:
    explicit Shift(int t, int d);

    int type; // 0 - Morning, 1 - Evening, 2 - Night, 3 - Free
    bool is_spare = false;
    int day;
    Nurse * nurse;
};


#endif //NSP_SHIFT_HPP

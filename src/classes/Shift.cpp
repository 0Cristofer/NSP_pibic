/* Nurse scheduling shift class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 08/02/2019
   Edited: 16/02/2019 */


#include "include/Shift.hpp"

Shift::Shift(int t, int d) : type(t), day(d) {
    if(t == 3) is_spare = true;
}

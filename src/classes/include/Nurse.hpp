/* Nurse scheduling nurse and state class.
   Author: Cristofer Oswald (cristoferoswald@gmail.com)
   Created: 23/11/2018
   Edited: 15/03/2019 */

#ifndef NSP_NURSE_HPP
#define NSP_NURSE_HPP

#include <vector>

class Shift;

/**
 * Stores a nurse state (shifts assigned, hard and soft constrains, etc)
 * and its previous state
 */
class State{
public:
    /**
     * Constructs a state based on a existing state
     * @param state The existing state
     */
    explicit State(State* state);

    State() = default;

    int days_worked = 0;
    int con_working_shifts = 0;
    int con_same_working_shifts = 0;

    std::vector<int> number_assignments_shifts;
    std::vector<Shift*> assgined_shifts;

    bool reached_min_worked = false;
    bool reached_min_con_working = false;

    std::vector<bool> reached_min_same_assign;
    std::vector<bool> reached_min_assign;

    State* prev = nullptr;
};

class Nurse {
public:
    explicit Nurse(int n);

    int number;
    int min_shift_now = 0;

    std::vector<int> preferences;

    State* current_state = nullptr;

    /**
     * Assign a new shift to this nurse, creating a new state and updating its constraints
     * @param shift
     */
    void assignShift(Shift *shift);

    /**
     * Creates a new state for this nurse
     */
    void newState();

    /**
     * Clears the current state and set this nurse's state to the previous one
     */
    void getPrevState();

    /**
     * Checks if minimum constraints were reached
     * @return True if reached
     */
    int getNumConstraintsViolated();

    virtual ~Nurse();
};


#endif /* NSP_NURSE_HPP */

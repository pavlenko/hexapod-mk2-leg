#ifndef FSM_H
#define FSM_H

#include <avr/io.h>

typedef struct {
    uint8_t stateIN;
    uint8_t stateOUT;
    void (*stateExit) ();
    void (*stateEnter) ();
} FSMTransition;

class FSMClass {
private:
    FSMTransition *_transitions;
    uint8_t _state;
public:
    /**
     * @param transitions
     * @param state
     */
    void initialize(FSMTransition *transitions, uint8_t state);
};

extern FSMClass FSM;

#endif //FSM_H

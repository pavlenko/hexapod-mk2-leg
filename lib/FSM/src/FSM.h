#ifndef FSM_H
#define FSM_H

#include <stdint.h>

//TODO are need state table as restriction to transition between states

typedef struct {
    uint8_t stateIN;
    uint8_t stateOUT;
    void (*stateExit) ();
    void (*stateEnter) ();
} FSMTransition;

class FSMState {
private:
    /**
     * Callback for enter state
     */
    void (*onEnter) ();

    /**
     * Callback for exit state
     */
    void (*onExit) ();
public:
    /**
     * @param onEnter
     */
    explicit FSMState(void (*onEnter) ());

    /**
     * @param onEnter
     * @param onExit
     */
    explicit FSMState(void (*onEnter) (), void (*onExit) ());

    /**
     * call onEnter
     */
    inline void enter();

    /**
     * Call onExit
     */
    inline void exit();
};

class FSMClass {
    friend FSMState;
private:
    FSMState *prevState;
    FSMState *nextState;
    FSMTransition *_transitions;
    uint8_t _state;
public:
    /**
     * Init FSM with state
     *
     * @param state
     */
    void initialize(FSMState *state);

    /**
     * @param transitions
     * @param state
     */
    void initialize(FSMTransition *transitions, uint8_t state);
    void dispatch();
};

extern FSMClass FSM;

#endif //FSM_H

#ifndef FSM_H
#define FSM_H

#include <stdint.h>

class FSMClass;

class FSMState {
    friend FSMClass;
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
};

class FSMClass {
private:
    FSMState *prevState;
    FSMState *nextState;
public:
    /**
     * Init FSM without state
     */
    void initialize();

    /**
     * Init FSM with state
     *
     * @param state
     */
    void initialize(FSMState &state);

    /**
     * @param state
     */
    void transitionTo(FSMState &state);

    /**
     * @param state
     * @param immediate
     */
    void transitionTo(FSMState &state, bool immediate);

    /**
     * Dispatch not immediate transitions
     */
    void dispatch();
};

extern FSMClass FSM;

#endif //FSM_H

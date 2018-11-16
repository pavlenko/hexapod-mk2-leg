#include "FSM.h"

FSMState::FSMState(void (*onEnter) ()) {
    this->onEnter = onEnter;
}

FSMState::FSMState(void (*onEnter) (), void (*onExit) ()) {
    this->onEnter = onEnter;
    this->onExit  = onExit;
}

void FSMState::enter() {
    if (this->onEnter) {
        this->onEnter();
    }
}

void FSMState::exit() {
    if (this->onExit) {
        this->onExit();
    }
}

void FSMClass::initialize(FSMState *state) {
    prevState = state;
    nextState = state;
}

void FSMClass::initialize(FSMTransition *transitions, uint8_t state) {
    _transitions = transitions;
    _state       = state;
}

void FSMClass::dispatch() {
    if (prevState) {
        prevState->exit();
    }

    if (nextState) {
        nextState->enter();
    }
}

FSMClass FSM;
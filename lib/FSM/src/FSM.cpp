#include "FSM.h"

FSMState::FSMState() = default;

FSMState::FSMState(void (*onEnter) ()) {
    this->onEnter = onEnter;
}

FSMState::FSMState(void (*onEnter) (), void (*onExit) ()) {
    this->onEnter = onEnter;
    this->onExit  = onExit;
}

void FSMClass::initialize() {
    prevState = nullptr;
}

void FSMClass::initialize(FSMState &state) {
    prevState = nextState = &state;

    if (prevState->onEnter) {
        prevState->onEnter();
    }
}

void FSMClass::transitionTo(FSMState &state) {
    nextState = &state;
}

void FSMClass::transitionTo(FSMState &state, bool immediate) {
    nextState = &state;

    if (immediate) {
        this->dispatch();
    }
}

void FSMClass::dispatch() {
    if (prevState != nextState) {
        if (prevState && prevState->onExit) {
            prevState->onExit();
        }

        prevState = nextState;

        if (prevState->onEnter) {
            prevState->onEnter();
        }
    }
}

FSMClass FSM;
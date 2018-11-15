#include "FSM.h"

void FSMClass::initialize(FSMTransition *transitions, uint8_t state) {
    _transitions = transitions;
    _state       = state;
}

FSMClass FSM;
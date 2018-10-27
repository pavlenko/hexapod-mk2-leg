#include "ES.h"

#include <stdint.h>

ESClass::ESClass() {
    this->head  = 0;
    this->tail  = 0;
    this->count = 0;
}

void ESClass::attach(uint8_t code, void (*handler_ptr) ()) {
    if (code < ES_HANDLERS_MAX) {
        this->handlers[code] = handler_ptr;
    }
}

void ESClass::detach(uint8_t code) {
    if (code < ES_HANDLERS_MAX) {
        this->handlers[code] = ES_EVENT_NOP;
    }
}

void ESClass::trigger(uint8_t code) {
    if (code < ES_HANDLERS_MAX && this->count < ES_EVENTS_MAX) {
        this->events[this->tail] = code;
        this->tail++;

        if (this->tail == ES_EVENTS_MAX) {
            this->tail = 0;
        }

        this->count++;
    }
}

void ESClass::dispatch() {
    if (this->count > 0) {
        uint8_t code = this->events[this->head];

        this->count--;
        this->head++;

        if (this->head == ES_EVENTS_MAX) {
            this->head = 0;
        }

        if (this->handlers[code]) {
            this->handlers[code]();
        }
    }
}

ESClass ES;

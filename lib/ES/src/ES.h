#ifndef ES_H
#define ES_H

#include <stdint.h>

typedef void (*es_handler_t) ();

#ifndef ES_HANDLERS_MAX
#define ES_HANDLERS_MAX 30
#endif

#ifndef ES_EVENTS_MAX
#define ES_EVENTS_MAX 30
#endif

#define ES_EVENT_NOP 0

class ESClass
{
private:
    volatile uint8_t head;
    volatile uint8_t tail;
    volatile uint8_t count;
    volatile uint8_t events[ES_EVENTS_MAX];
    volatile es_handler_t handlers[ES_HANDLERS_MAX];
public:
    ESClass();

    /**
     * Attach handler for event, previous one will be replaced
     *
     * @param code
     * @param handler_ptr
     */
    void attach(uint8_t code, void (*handler_ptr) ());

    /**
     * Detach handler from event
     *
     * @param code
     */
    void detach(uint8_t code);

    /**
     * Trigger event
     *
     * @param code
     */
    void trigger(uint8_t code);

    /**
     * Dispatch triggered events
     */
    void dispatch();
};

extern ESClass ES;

#endif //ES_H

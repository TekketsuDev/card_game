#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdbool.h>

typedef enum {
    EVENT_INIT_GAME,
    EVENT_DRAW_CARD,
    EVENT_END_TURN,
    EVENT_INTERRUPT,
    EVENT_PLACE_CARDS
} StateEventType;

typedef struct {
    StateEventType type;
    // aquí puedes añadir payload del evento (jugador, carta, etc.)
    // int playerId;
    // int cardId;
} Event;

typedef struct ListNode {
    Event elem;
    struct ListNode *prev;
    struct ListNode *next;
} ListNode;

typedef struct {
    ListNode *head;
    ListNode *tail;
    int size;

    StateEventType current_state;
} StateMachine;

// API pública
void initStateMachine(StateMachine *sm);
void shutdownStateMachine(StateMachine *sm);

Event makeEvent(StateEventType type);

bool pushEvent(StateMachine *sm, Event ev);
bool popEvent(StateMachine *sm, Event *out);   // devuelve false si la cola está vacía

void processNextEvent(StateMachine *sm);       // consume un evento y actualiza estado

#endif


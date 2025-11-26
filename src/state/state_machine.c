#include <stdlib.h>
#include "state_machine.h"

void initStateMachine(StateMachine *sm) {
    if (!sm) return;

    sm->head = NULL;
    sm->tail = NULL;
    sm->size = 0;

    sm->current_state = EVENT_INIT_GAME;

    // Encolamos el evento inicial
    pushEvent(sm, makeEvent(EVENT_INIT_GAME));
}

void shutdownStateMachine(StateMachine *sm) {
    if (!sm) return;

    Event dummy;
    while (popEvent(sm, &dummy)) {
        // solo vaciamos la cola
    }
}

Event makeEvent(StateEventType type) {
    Event e;
    e.type = type;
    return e;
}

bool pushEvent(StateMachine *sm, Event ev) {
    if (!sm) return false;

    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (!node) return false;

    node->elem = ev;
    node->next = NULL;
    node->prev = sm->tail;

    if (sm->tail) {
        sm->tail->next = node;
    } else {
        sm->head = node;
    }

    sm->tail = node;
    sm->size++;

    return true;
}

bool popEvent(StateMachine *sm, Event *out) {
    if (!sm || sm->size == 0 || !sm->head) return false;

    ListNode *node = sm->head;
    if (out) *out = node->elem;

    sm->head = node->next;
    if (sm->head) {
        sm->head->prev = NULL;
    } else {
        sm->tail = NULL;
    }

    free(node);
    sm->size--;

    return true;
}

// Ojo: la lógica de juego NO va aquí.
static void handleEvent(StateMachine *sm, const Event *ev) {
    // De momento solo reflejamos el último evento procesado.
    sm->current_state = ev->type;
}

void processNextEvent(StateMachine *sm) {
    if (!sm) return;

    Event ev;
    if (!popEvent(sm, &ev)) return;

    handleEvent(sm, &ev);
}


#include "raylib.h"

initStateMachine(StateMachine *sm) {

}

#include <stdlib.h>
#include "state_machine.h"

void initStateMachine(StateMachine *sm) {
    if (!sm) return;

    sm->head = NULL;
    sm->tail = NULL;
    sm->size = 0;

    sm->current_state = EVENT_INIT_GAME;

    pushEvent(sm, makeEvent(EVENT_INIT_GAME));
}

void shutdownStateMachine(StateMachine *sm) {
    if (!sm) return;

    // vaciar la cola y liberar memoria
    Event dummy;
    while (popEvent(sm, &dummy)) {
        // nada que hacer, solo limpiamos nodos
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
        // cola vacía, este nodo es también el head
        sm->head = node;
    }

    sm->tail = node;
    sm->size++;

    return true;
}

bool popEvent(StateMachine *sm, Event *out) {
    if (!sm || sm->size == 0 || !sm->head) return false;

    ListNode *node = sm->head;
    if (out) {
        *out = node->elem;
    }

    sm->head = node->next;
    if (sm->head) {
        sm->head->prev = NULL;
    } else {
        // la cola se ha vaciado
        sm->tail = NULL;
    }

    free(node);
    sm->size--;

    return true;
}

static void handleEvent(StateMachine *sm, const Event *ev) {
    switch (ev->type) {
        case EVENT_INIT_GAME:

            Zones zones;
            Zones_Layout(&zones, CARD_SIZE);
  
            // Baraja + mano
            Deck deck;
            Hand hand;
            InitStandardDeck(&deck, CARD_SIZE);
            ShuffleDeck(&deck);

            
            // TODO: barajar mazo, repartir cartas, resetear estado de jugadores, etc.
            // Después de inicializar, cambiamos de estado:
            sm->current_state = EVENT_DRAW_CARD;
            break;

        case EVENT_DRAW_CARD:
            // TODO: lógica de robar carta (jugador actual, mazo, etc.)
            // Posible transición:
            // sm->current_state = EVENT_END_TURN;
            break;

        case EVENT_END_TURN:
            // TODO: pasar al siguiente jugador, comprobar fin de partida...
            // Posible transición:
            // sm->current_state = EVENT_DRAW_CARD;
            break;

        case EVENT_INTERRUPT:
            // TODO: manejar interrupciones (hechizos, cartas trampa, etc.)
            // sm->current_state = ...;
            break;

        default:
            // evento desconocido: de momento, no hacemos nada
            break;
    }
}

void processNextEvent(StateMachine *sm) {
    if (!sm) return;

    Event ev;
    if (!popEvent(sm, &ev)) {
        // no hay eventos pendientes
        return;
    }

    handleEvent(sm, &ev);
}


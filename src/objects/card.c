#include "card.h"

// Color base por palo (ajústalo a tu gusto)
Color SuitBaseColor(Suit s) {
    switch (s) {
        case SUIT_HEARTS:   return (Color){230,  70,  60, 255}; // rojo
        case SUIT_DIAMONDS: return (Color){240, 190,  60, 255}; // dorado
        case SUIT_CLUBS:    return (Color){ 60, 200, 110, 255}; // verde
        case SUIT_SPADES:   return (Color){120, 120, 230, 255}; // azul/violeta
        default:            return RAYWHITE;
    }
}

Color ColorForCard(Suit s, int rank) {
    if (rank < 1) rank = 1;
    if (rank > 13) rank = 13;
    Color base  = SuitBaseColor(s);
    float rf    = (float)(rank - 1) / 12.0f;
    float mix   = 0.25f * rf;
    Color white = (Color){255,255,255,255};
    Color c = ColorLerp(base, white, mix);
    return c;
}

Card MakeCard(Suit s, int rank, Vector2 pos, Vector2 size) {
    Card c = {
        .position   = pos,
        .size       = size,
        .color      = ColorForCard(s, rank),
        .draggable  = false,
        .dragOffset = (Vector2){0,0},
        .prevPos    = (Vector2){0,0},
        .handSlot   = -1,
        .suit       = s,
        .rank       = rank
    };
    return c;
}

void DrawCard(const Card *c) {
    DrawRectangleV(c->position, c->size, c->color);
    DrawRectangleLines((int)c->position.x, (int)c->position.y,
                       (int)c->size.x, (int)c->size.y, BLACK);
}

// ---------------- Deck & Hand ----------------

void InitStandardDeck(Deck *d, Vector2 cardSize) {
    int idx = 0;
    for (int s = 0; s < 4; ++s) {
        for (int r = 1; r <= 13; ++r) {
            d->cards[idx++] = MakeCard((Suit)s, r, (Vector2){0,0}, cardSize);
        }
    }
    d->top = DECK_SIZE;
}

// Fisher–Yates
void ShuffleDeck(Deck *d) {
    for (int i = d->top - 1; i > 0; --i) {
        int j = GetRandomValue(0, i); 
        Card t = d->cards[i];
        d->cards[i] = d->cards[j];
        d->cards[j] = t;
    }
}

int DeckDrawOne(Deck *d, Card *out) {
    if (d->top <= 0) return 0;
    *out = d->cards[--d->top];
    return 1;
}

void HandInit(Hand *h) { h->count = 0; }

int HandAdd(Hand *h, Card c) {
    if (h->count >= MAX_HAND) return 0;
    c.handSlot = h->count;
    h->cards[h->count++] = c;
    return 1;
}

void HandRemoveAt(Hand *h, int i) {
    if (i < 0 || i >= h->count) return;
    for (int k = i; k < h->count - 1; ++k) {
        h->cards[k] = h->cards[k + 1];
        h->cards[k].handSlot = k;
    }
    h->count--;
}

void LayoutHandRow(Hand *h, Rectangle zone, Vector2 size, float spacing) {
    if (h->count <= 0) return;
    float total  = h->count * size.x + (h->count - 1) * spacing;
    float startX = zone.x + (zone.width  - total) * 0.5f;
    float y      = zone.y + (zone.height - size.y) * 0.5f;
    for (int i = 0; i < h->count; ++i) {
        h->cards[i].position = (Vector2){ startX + i * (size.x + spacing), y };
    }
}


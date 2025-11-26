#ifndef CARD_H_
#define CARD_H_

#include "raylib.h"
#include <stdlib.h>

#define DECK_SIZE 12

#ifndef MAX_HAND 
#define MAX_HAND 8
#endif

typedef enum Suit {
    SUIT_HEARTS = 0,
    SUIT_DIAMONDS,
    SUIT_CLUBS,
    SUIT_SPADES
} Suit;

typedef struct Card {
  Vector2 position;
  Vector2 size;
  Color color;
  bool draggable;
  Vector2 dragOffset;
  Vector2 prevPos;
  int handSlot;
  Suit suit;
  int rank;
} Card;

typedef struct Deck {
    Card cards[DECK_SIZE];
    int top;
} Deck;

typedef struct Hand {
    Card cards[MAX_HAND];
    int count;
} Hand;

typedef struct Board {
    Card cards[MAX_HAND];
    int count;
} Board;

typedef struct Graveyard {
    Card cards[DECK_SIZE];
    int count;
} Graveyard;

#ifdef __cplusplus
extern "C" {
#endif

// Generación de color y construcción de cartas
Color SuitBaseColor(Suit s);
Color ColorForCard(Suit s, int rank);
Card  MakeCard(Suit s, int rank, Vector2 pos, Vector2 size);

// Dibujo
void DrawCard(const Card *c);

// Deck / Hand
void InitStandardDeck(Deck *d, Vector2 cardSize);
void ShuffleDeck(Deck *d);
int  DeckDrawOne(Deck *d, Card *out);

void HandInit(Hand *h);
int  HandAdd(Hand *h, Card c);
void HandRemoveAt(Hand *h, int i);
void LayoutHandRow(Hand *h, Rectangle zone, Vector2 size, float spacing);

#ifdef __cplusplus
}
#endif

#endif // CARD_H_


#ifndef ZONE_H_
#define ZONE_H_

#include "raylib.h"
#include "card.h"

typedef struct Zone {
    const char* name;
    Rectangle   rect;
    Color       bg;
    Color       border;
} Zone;

typedef struct Zones {
    Zone boss;
    Zone play;
    Zone hand;
    Zone deck;
    Zone enemyDeck;
} Zones;

void Zones_Layout(Zones* z, Vector2 cardSize);

void Zones_Draw(const Zones* z);

static inline Rectangle CardRect(const Card* c) {
    return (Rectangle){ c->position.x, c->position.y, c->size.x, c->size.y };
}
bool CardInZone(const Card* c, const Zone* z);
void ClampCardToZone(Card* c, const Zone* z);

void DrawZoneBox(const Zone* z);

#endif


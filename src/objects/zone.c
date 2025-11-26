#include "zone.h"
#include <stdio.h>

static void LayoutTwo(Zones* z, Vector2 cardSize) {
    int w = GetScreenWidth(), h = GetScreenHeight();
    
    float hHand = cardSize.y + 70.0f;
    float hMid  = (float)h  - hHand;
    float wDeck = cardSize.x + 40.0f;
    float hDeck = cardSize.y + 60.0f;
    float margin   = 10.0f;

    float maxDeckW = ((float)w - 3.0f*margin) * 0.5f;
    if (wDeck > maxDeckW) wDeck = maxDeckW;

    float maxDeckH = (hMid - 2.0f*margin);
    if (hDeck > maxDeckH) hDeck = maxDeckH;



    z->play = (Zone){ "PLAY", (Rectangle){ 0, 0, (float)w, hMid }, GetColor(0x22332233), DARKGREEN,0 };
    z->hand = (Zone){ "HAND", (Rectangle){ 0, (float)h - hHand, (float)w, hHand }, GetColor(0x2a1b1f55), MAROON,0 };
    z->deck = (Zone){ "DECK", (Rectangle){ margin,h/2 , wDeck, hDeck },GetColor(0x2a1b1f55), RAYWHITE, 1 };
    z->enemyDeck = (Zone){ "ENEMY DECK",(Rectangle){ w - margin - wDeck, h/2, wDeck, hDeck }, GetColor(0x2a1b1f55), GREEN, 1 };

// z->enemyDeck = (Zone){ "ENEMY DECK",(Rectangle){ (float)w - margin - wDeck, yMidCenter, wDeck, hDeck }, GetColor(0x2a1b1f55), GREEN };
}


void Zones_Layout(Zones* z, Vector2 cardSize) {
    if (!z) return;
    LayoutTwo(z, cardSize);
}

void DrawZoneBox(const Zone* z) {
    DrawRectangleRec(z->rect, z->bg);
    DrawRectangleLinesEx(z->rect, 2, z->border);
    DrawText(z->name, (int)(z->rect.x + 8), (int)(z->rect.y + 8), 20, z->border);
}

void Zones_Draw(const Zones* z) {
    if (!z) return;
    DrawZoneBox(&z->deck);
    DrawZoneBox(&z->play);
    DrawZoneBox(&z->hand);
    DrawZoneBox(&z->enemyDeck);
}

bool CardInZone(const Card* c, const Zone* z) {
    return CheckCollisionRecs(CardRect(c), z->rect);
}

void ClampCardToZone(Card* c, const Zone* z) {
    if (!c || !z) return;
    if (c->position.x < z->rect.x) c->position.x = z->rect.x;
    if (c->position.y < z->rect.y) c->position.y = z->rect.y;
    float maxX = z->rect.x + z->rect.width  - c->size.x;
    float maxY = z->rect.y + z->rect.height - c->size.y;
    if (c->position.x > maxX) c->position.x = maxX;
    if (c->position.y > maxY) c->position.y = maxY;
}



const Zone* Zones_GetZoneAtPoint(const Zones* zs, Vector2 p) {
    const Zone* zones[] = {
        &zs->boss, &zs->play, &zs->hand, &zs->deck, &zs->enemyDeck
    };

    const Zone* best = NULL;
    int bestLayer = -9999;

    // encuentra todas las colisiones y elige la de mayor layer
    for (int i = 0; i < 5; i++) {
        const Zone* z = zones[i];
        if (CheckCollisionPointRec(p, z->rect)) {
            if (z->layer > bestLayer) {
                best = z;
                bestLayer = z->layer;
            }
        }
    }

    // log igual que tu código original
    static const Zone* last = NULL;

    if (best != last) {
        if (best != NULL && last == NULL) {
            printf("Mouse entered zone: %s\n", best->name);
        } else if (best != NULL && last != NULL) {
            printf("Mouse changed zone: %s -> %s\n", last->name, best->name);
        } else if (best == NULL && last != NULL) {
            printf("Mouse left zone: %s\n", last->name);
        }
        last = best;
    }

    return best;
}

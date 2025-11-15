#include "zone.h"

static void LayoutOne(Zones* z, Vector2 cardSize) {
    int w = GetScreenWidth(), h = GetScreenHeight();

    float hBoss = cardSize.y + 70.0f;
    float hHand = cardSize.y + 70.0f;
    float hMid  = (float)h - hBoss - hHand;
    float wDeck = cardSize.x + 40.0f;
    float hDeck = cardSize.y + 60.0f;
    float margin   = 10.0f;

    float maxDeckW = ((float)w - 3.0f*margin) * 0.5f;
    if (wDeck > maxDeckW) wDeck = maxDeckW;

    float maxDeckH = (hMid - 2.0f*margin);
    if (hDeck > maxDeckH) hDeck = maxDeckH;

    float yMidTop    = hBoss + margin;
    float yMidBottom = hBoss + hMid - margin;
    float yMidCenter = yMidTop + ((yMidBottom - yMidTop) - hDeck) * 0.5f;

    z->boss = (Zone){ "BOSS", (Rectangle){ 0, 0, (float)w, hBoss }, GetColor(0x1b1f2a55), DARKBLUE };
    z->play = (Zone){ "PLAY", (Rectangle){ 0, hBoss, (float)w, hMid }, GetColor(0x22332233), DARKGREEN };
    z->hand = (Zone){ "HAND", (Rectangle){ 0, (float)h - hHand, (float)w, hHand }, GetColor(0x2a1b1f55), MAROON };

    z->deck = (Zone){ "DECK",
        (Rectangle){ margin, yMidCenter, wDeck, hDeck },
        GetColor(0x2a1b1f55), RAYWHITE
    };
    z->enemyDeck = (Zone){ "ENEMY DECK",
        (Rectangle){ (float)w - margin - wDeck, yMidCenter, wDeck, hDeck },
        GetColor(0x2a1b1f55), DARKBROWN
    };
}

static void LayoutTwo(Zones* z, Vector2 cardSize) {
    int w = GetScreenWidth(), h = GetScreenHeight();
    
    float hBoss = cardSize.y + 70.0f;
    float hHand = cardSize.y + 70.0f;
    float hMid  = (float)h  - hHand;
    float wDeck = cardSize.x + 40.0f;
    float hDeck = cardSize.y + 60.0f;
    float margin   = 10.0f;

    float maxDeckW = ((float)w - 3.0f*margin) * 0.5f;
    if (wDeck > maxDeckW) wDeck = maxDeckW;

    float maxDeckH = (hMid - 2.0f*margin);
    if (hDeck > maxDeckH) hDeck = maxDeckH;

    float yMidTop    = hBoss + margin;
    float yMidBottom = hBoss + hMid - margin;
    float yMidCenter = yMidTop + ((yMidBottom - yMidTop) - hDeck) * 0.5f;

    z->play = (Zone){ "PLAY", (Rectangle){ 0, 0, (float)w, hMid }, GetColor(0x22332233), DARKGREEN };
    z->hand = (Zone){ "HAND", (Rectangle){ 0, (float)h - hHand, (float)w, hHand }, GetColor(0x2a1b1f55), MAROON };

    z->deck = (Zone){ "DECK",
        (Rectangle){ margin, yMidCenter, wDeck, hDeck },
        GetColor(0x2a1b1f55), RAYWHITE
    };
    z->enemyDeck = (Zone){ "ENEMY DECK",
        (Rectangle){ (float)w - margin - wDeck, yMidCenter, wDeck, hDeck },
        GetColor(0x2a1b1f55), DARKBROWN
    };
}


void Zones_Layout(Zones* z, Vector2 cardSize) {
    if (!z) return;
    //LayoutOne(z, cardSize);
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


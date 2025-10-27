#include "raylib.h"
//#include "resource_dir.h"
#include "card.h"
#include "card_animation.h"
#include "zone.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static const Vector2 CARD_SIZE = {100.0f, 150.0f};

static void ScreenConfig(void) {
   SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    int mon = 0;
    int scrW = GetMonitorWidth(mon), scrH = GetMonitorHeight(mon);
    int winW = (int)(scrW * 0.85f), winH = (int)(scrH * 0.85f);
    if (winW < 1024) winW = 1024;
    if (winH < 720)  winH = 720;
    InitWindow(winW, winH, "card game");
    SetWindowMinSize(900, 600);
    SetTargetFPS(60);
    //SearchAndSetResourceDir("resources");
    srand((unsigned)time(NULL));
}


int main(void) {
    // Ventana
    ScreenConfig();

    Zones zones;
    Zones_Layout(&zones, CARD_SIZE);

    // Baraja + mano
    Deck deck;
    Hand hand;
    InitStandardDeck(&deck, CARD_SIZE);
    ShuffleDeck(&deck);

    HandInit(&hand);
    // Roba 7 cartas iniciales
    for (int i = 0; i < 7; ++i) {
        Card c;
        if (DeckDrawOne(&deck, &c)) HandAdd(&hand, c);
    }
    LayoutHandRow(&hand, zones.hand.rect, CARD_SIZE, 20.0f);

    ParticleEmitter emit[MAX_HAND];
    for (int i = 0; i < MAX_HAND; ++i) InitParticles(&emit[i]);

    int dragging = -1;
    Vector2 dragOffset = {0,0};

    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            Zones_Layout(&zones,CARD_SIZE);
            LayoutHandRow(&hand, zones.hand.rect, CARD_SIZE, 20.0f);
        }

        Vector2 mPos = GetMousePosition();
      // pick
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = hand.count - 1; i >= 0; --i) {
                Rectangle r = CardRect(&hand.cards[i]);
                if (CheckCollisionPointRec(mPos, r)) {
                    dragging = i;
                    dragOffset = (Vector2){ mPos.x - hand.cards[i].position.x,
                                            mPos.y - hand.cards[i].position.y };
                    break;
                }
            }
        }

        // drag
        if (dragging != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            hand.cards[dragging].position = (Vector2){ mPos.x - dragOffset.x, mPos.y - dragOffset.y };
        }

        // drop
        if (dragging != -1 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            bool played = CardInZone(&hand.cards[dragging], &zones.play);
            if (played) {
                ClampCardToZone(&hand.cards[dragging], &zones.play);
                HandRemoveAt(&hand, dragging);
                for (int k = dragging; k < MAX_HAND - 1; ++k) emit[k] = emit[k + 1];
                Card c; if (DeckDrawOne(&deck, &c)) { HandAdd(&hand, c); 
                InitParticles(&emit[hand.count - 1]); }
                LayoutHandRow(&hand, zones.hand.rect, CARD_SIZE, 20.0f);
            } else {
                ClampCardToZone(&hand.cards[dragging], &zones.hand);
            }
            dragging = -1;
        }

        // Render
        BeginDrawing();
        ClearBackground(WHITE);

         Zones_Draw(&zones);

              DrawRectangle((int)(zones.deck.rect.x + 12), (int)(zones.deck.rect.y + 12),
                      (int)CARD_SIZE.x, (int)CARD_SIZE.y, DARKGRAY);
        DrawRectangleLines(
                (int)(zones.deck.rect.x + 12),
                (int)(zones.deck.rect.y + 12),
                (int)CARD_SIZE.x,
                (int)CARD_SIZE.y,
                BLACK);
        DrawText(TextFormat("%d", deck.top), (int)(zones.deck.rect.x + 18), (int)(zones.deck.rect.y + 18), 20, BLACK);
        // Cartas de la mano
        for (int i = 0; i < hand.count; ++i) {
            DrawCard(&hand.cards[i]);
        }

        // Partículas por carta (recortadas a la carta)
        float dt = GetFrameTime();
        for (int i = 0; i < hand.count; ++i) {
            bool inPlay = CardInZone(&hand.cards[i], &zones.play);
            int spawns = inPlay ? 3 : 1;
            for (int k = 0; k < spawns; ++k) {
                CreateParticleAtObject(&emit[i], &hand.cards[i]);
            }
            ParticleUpdate(&emit[i], dt);
            ParticleDraw(&emit[i], &hand.cards[i]);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}


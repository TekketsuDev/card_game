#include "raylib.h"
#include "resource_dir.h"
#include "object.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#define OBJECT_COUNT 11
#define MAX_HAND 8

/* Color Cards */
static const Color COLORS[] = {
    LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED,
    MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, BLUE, DARKBLUE,
    PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
    WHITE, BLACK, MAGENTA, RAYWHITE
};

#define COLOR_COUNT (sizeof(COLORS)/sizeof(COLORS[0]))

int screenWidth = 1800;
 int screenHeight = 1200;
static const Vector2 CARD_SIZE = {100.0f, 150.0f};
Color GetRandColor(void) {
    return COLORS[rand() % COLOR_COUNT];
}
/* End Color Cards */



/* Zone Cards */
typedef struct Zone {
    const char* name;
    Rectangle rect;
    Color bg;
    Color border;
} Zone;

static Zone ZBoss, ZPlay, ZHand, ZDeck, ZEnemyDeck;

static void LayoutZones(void) {
    int w = GetScreenWidth(), h = GetScreenHeight();
    float hBoss = CARD_SIZE.y + 70.0f;
    float hHand = CARD_SIZE.y + 70.0f;
    float hMid  = h - hBoss - hHand;
    float wDeck = CARD_SIZE.x + 40.0f;
    float hDeck = CARD_SIZE.y + 60.0f;
    float margin   = 10.0f;

    float maxDeckW = (w - 3*margin) * 0.5f;
    if (wDeck > maxDeckW) wDeck = maxDeckW;

    float maxDeckH = (hMid - 2*margin);
    if (hDeck > maxDeckH) hDeck = maxDeckH;

    float yMidTop    = hBoss + margin;
    float yMidBottom = hBoss + hMid - margin;
    float yMidCenter = yMidTop + ((yMidBottom - yMidTop) - hDeck) * 0.5f;

    ZBoss = (Zone){ "BOSS", (Rectangle){ 0, 0, w, hBoss }, GetColor(0x1b1f2a55), DARKBLUE };
    ZPlay = (Zone){ "PLAY", (Rectangle){ 0, hBoss, w, hMid }, GetColor(0x22332233), DARKGREEN };
    ZHand = (Zone){ "HAND", (Rectangle){ 0, h - hHand, w, hHand }, GetColor(0x2a1b1f55), MAROON };

    ZDeck      = (Zone){ "DECK",        (Rectangle){ margin, yMidCenter, wDeck, hDeck }, GetColor(0x2a1b1f55), RAYWHITE   };
    ZEnemyDeck = (Zone){ "ENEMY DECK",  (Rectangle){ w - margin - wDeck, yMidCenter, wDeck, hDeck }, GetColor(0x2a1b1f55), DARKBROWN };
}

static void DrawZone(const Zone* z) {
    DrawRectangleRec(z->rect, z->bg);
    DrawRectangleLinesEx(z->rect, 2, z->border);
    DrawText(z->name, z->rect.x + 8, z->rect.y + 8, 20, z->border);
}

static Rectangle CardRect(const Object* o) {
    return (Rectangle){ o->position.x, o->position.y, o->size.x, o->size.y };
}

static bool CardInZone(const Object* o, const Zone* z) {
    return CheckCollisionRecs(CardRect(o), z->rect);
}

static void ClampCardToZone(Object* o, const Zone* z) {
    if (o->position.x < z->rect.x) o->position.x = z->rect.x;
    if (o->position.y < z->rect.y) o->position.y = z->rect.y;
    float maxX = z->rect.x + z->rect.width  - o->size.x;
    float maxY = z->rect.y + z->rect.height - o->size.y;
    if (o->position.x > maxX) o->position.x = maxX;
    if (o->position.y > maxY) o->position.y = maxY;
}

static void LayoutHandRow(Object hand[], int n) {
    float spacing = 20.0f;
    float total = n*CARD_SIZE.x + (n-1)*spacing;
    float startX = (GetScreenWidth() - total) * 0.5f;
    float y = ZHand.rect.y + (ZHand.rect.height - CARD_SIZE.y) * 0.5f;
    for (int i = 0; i < n; ++i)
        hand[i].position = (Vector2){ startX + i*(CARD_SIZE.x + spacing), y };
}
/* End Zone Cards */



int main (void) {
    
     
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI); 
    InitWindow(screenWidth, screenHeight, "card game");
    SearchAndSetResourceDir("resources"); 
    srand((unsigned)time(NULL)); 
    SetTargetFPS(60); 
    LayoutZones();
  /* End Monitor and Screen Resolution */


  Object deck[OBJECT_COUNT];
  Object hand[MAX_HAND];
  
  for (int i = 0; i < OBJECT_COUNT; i++) {

    deck[i] = CreateObject(
      (Vector2) {i*15, i*25},
      CARD_SIZE,
      GetRandColor()
    );
  }
  for (int i = 0; i < MAX_HAND; i++) {
    hand[i] = CreateObject(
      (Vector2) {0,0},
      CARD_SIZE,
      GetRandColor()
    );
  }

    LayoutHandRow(hand, MAX_HAND); 
    int dragging = -1; 
    Vector2 dragOffset = {0,0};

	while (!WindowShouldClose()) {
        
        //if (IsWindowResized()) { LayoutZones(); LayoutHandRow(hand, MAX_HAND); }
        Vector2 mPos = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            for (int i = MAX_HAND - 1; i >= 0; --i) {
                Rectangle r = (Rectangle){ hand[i].position.x, hand[i].position.y, hand[i].size.x, hand[i].size.y };
                if (CheckCollisionPointRec(mPos, r)) {
                    dragging = i;
                    dragOffset = (Vector2){ mPos.x - hand[i].position.x, mPos.y - hand[i].position.y };
                    break;
                }
            }
        }
    if (dragging != -1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        hand[dragging].position = (Vector2){ mPos.x - dragOffset.x, mPos.y - dragOffset.y };
        }
        if (dragging != -1 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (!CardInZone(&hand[dragging], &ZPlay)) {
                ClampCardToZone(&hand[dragging], &ZHand);
            } else {
                ClampCardToZone(&hand[dragging], &ZPlay);
            }
            dragging = -1;
        }
	BeginDrawing();

	ClearBackground(White);
    DrawZone(&ZBoss);    
    DrawZone(&ZDeck);
    DrawZone(&ZPlay);
    DrawZone(&ZHand);;
    DrawZone(&ZEnemyDeck);
    
/*
    for (int i = 0; i < OBJECT_COUNT; i++) {
      DrawObject(deck[i]);
    }
*/
    for (int i = 0; i < MAX_HAND; i++) {
      DrawObject(hand[i]);
    }
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

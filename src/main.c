#include "raylib.h"
#include "resource_dir.h"
#include "object.h"
#include "time.h"
#include "stdlib.h"

#define OBJECT_COUNT 11

int screen Width = 1800;
int screenHeight = 1200;

static const Color COLORS[] = {
    LIGHTGRAY, GRAY, DARKGRAY, YELLOW, GOLD, ORANGE, PINK, RED,
    MAROON, GREEN, LIME, DARKGREEN, SKYBLUE, BLUE, DARKBLUE,
    PURPLE, VIOLET, DARKPURPLE, BEIGE, BROWN, DARKBROWN,
    WHITE, BLACK, MAGENTA, RAYWHITE
};

#define COLOR_COUNT (sizeof(COLORS)/sizeof(COLORS[0]))

int main () {

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	InitWindow(screenWidth, screenHeight, "card game");
	SearchAndSetResourceDir("resources");

  Color GetRandColor(void) {
    return COLORS[rand() % COLOR_COUNT];
  }

  Object deck[OBJECT_COUNT];
  Object hand[MAX_HAND];
  
  for (int i = 0; i < OBJECT_COUNT; i++) {
    deck[i] = CreateObject(
      (Vector2) {i*15, i*25},
      (Vector2) {100, 150},
      GetRandColor()
    );
  }
  for (int i = 0; i < MAX_HAND; i++) {
    hand[i] = CreateObject(
      (Vector2) {screenWidth/2,screenHeight },
      (Vector2) {100, 150},
      GetRandColor()
    );
  }

  Object o = CreateObject(
    (Vector2) {screenWidth/2, screenHeight  - 50 - 150},
    (Vector2) {100, 150},
    BLACK
  );

    Object e = CreateObject(
    (Vector2) {screenWidth/2 +120, screenHeight  - 200},
    (Vector2) {100, 150},
    BLUE
  );

    Object t = CreateObject(
    (Vector2) { screenWidth/2 - 120, screenHeight - 200},
    (Vector2) {100, 150},
    VIOLET
  );

	while (!WindowShouldClose())
	{
    Vector2 mPos = GetMousePosition;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
      for (int i = 0; i < MAX_HAND; i++) {
        Object obj = hand[i];

        Rectangle handR = {
          hand.position.x,
          hand.position.y,
          hand.size.x,
          hand.size.y,
        };
        if (CheckCollisionPointRec(mPos,))
      }
    }

		BeginDrawing();

		ClearBackground(WHITE);
    for (int i = 0; i < OBJECT_COUNT; i++) {
      DrawObject(deck[i]);
    }
    for (int i = 0; i < MAX_HAND; i++) {
      DrawObject(hand[i]);
    }
		EndDrawing();
	}

	CloseWindow();
	return 0;
}

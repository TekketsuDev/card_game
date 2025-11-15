#include "raylib.h"
//#include "resource_dir.h"
#include "card.h"
#include "card_animation.h"
#include "zone.h"
//#include "utilities.h"
//#include "state_machine.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

static const Vector2 CARD_SIZE = {100.0f, 150.0f};


static void ScreenConfig(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
  const int screenWidth = 1920;
    const int screenHeight = 1080;

    InitWindow(screenWidth, screenHeight, "raylib [models] example - waving cubes");
  /*
  int mon = 0;
  int scrW = GetMonitorWidth(mon), scrH = GetMonitorHeight(mon);
  int winW = (int)(scrW * 0.85f), winH = (int)(scrH * 0.85f);
  if (winW < 1024) winW = 1024;
  if (winH < 720)  winH = 720;
  InitWindow(winW, winH, "card game");
  SetWindowMinSize(900, 600);
  */
  SetTargetFPS(60);
  //SearchAndSetResourceDir("resources");
  srand((unsigned)time(NULL));
}


int main(void) {

  Camera3D camera = { 0 };
  camera.position = (Vector3){ -3.0f, 3.0f, -3.0f }; // Camera position
  camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
  camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
  camera.fovy = 25.0f;                                // Camera field-of-view Y
  camera.projection = CAMERA_FREE;

  Vector3 cubePosition = { 0.0f, 0.0f, 0.0f };
  
  const int GRID_SLICE = 10;
  const int GRID_COLS = 20;
  const int GRID_ROWS = 20;
  int gridX = 0;
  int gridZ = 0;


  // Ventana
  ScreenConfig();
  //stateMachine *sm;
  //initStateMachine(&sm);


  //while (!WindowShouldClose()) {

  //}
  //  --------------------------------------------------------------------------------------------- 
  /*
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
  */
    
  while (!WindowShouldClose()) {
    /*
    if (IsWindowResized()) {
      Zones_Layout(&zones,CARD_SIZE);
      LayoutHandRow(&hand, zones.hand.rect, CARD_SIZE, 20.0f);
    }

    Vector2 mPos = GetMousePosition();

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
      camera.fovy += 10;
    }
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      camera.fovy -= 10;
    }
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

    */
    // ------------------------------

    if (IsKeyPressed(KEY_RIGHT)) gridX++;
    if (IsKeyPressed(KEY_LEFT))  gridX--;
    if (IsKeyPressed(KEY_DOWN))  gridZ++;
    if (IsKeyPressed(KEY_UP))    gridZ--;

    //Colilision 
    if (gridX < 0)             gridX = 0;
    if (gridX >= GRID_COLS)    gridX = GRID_COLS - 1;
    if (gridZ < 0)             gridZ = 0;
    if (gridZ >= GRID_ROWS)    gridZ = GRID_ROWS - 1;

     
    cubePosition.x = gridX;
    cubePosition.z = gridZ;

  


    // Render
    BeginDrawing();

    //Zones_Draw(&zones);


    BeginMode3D(camera);
    for (int i = 0; i < GRID_COLS;i++){
      for(int j = 0; j < GRID_ROWS;j++){
        cubePosition.x = i;
        cubePosition.z = j;
        DrawCube(cubePosition,1.0f, 1.0f,1.0f, BLUE);
    DrawCubeWires(cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
      }

    }
    //DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, RED);

    DrawGrid(GRID_SLICE, 2.0f);

    EndMode3D();


    /*
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
    */
    EndDrawing();
  }
  CloseWindow();
  return 0;
}


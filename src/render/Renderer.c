#include "Renderer.h"
#include "raylib.h"
#include <stdio.h>

void Renderer_Draw(GameContext *ctx) {
    if (!ctx) return;
    
    const int GRID_COLS = 20;
    const int GRID_ROWS = 20;
    const int GRID_SLICE = 10;


    BeginDrawing();
    ClearBackground(RAYWHITE);

    BeginMode3D(ctx->camera);
    for (int i = 0; i < GRID_COLS;i++){
      for(int j = 0; j < GRID_ROWS;j++){
        ctx->cubePosition.x = i;
        ctx->cubePosition.z = j;
        DrawCube(ctx->cubePosition,1.0f, 1.0f,1.0f, BLUE);
    DrawCubeWires(ctx->cubePosition, 2.0f, 2.0f, 2.0f, MAROON);
      }

    }
    //DrawCube(cubePosition, 1.0f, 1.0f, 1.0f, RED);

    DrawGrid(GRID_SLICE, 2.0f);

    EndMode3D();
    Zones_Draw(&ctx->zones);

    DrawRectangle(
        (int)(ctx->zones.deck.rect.x + 12),
        (int)(ctx->zones.deck.rect.y + 12),
        (int)ctx->cardSize.x,
        (int)ctx->cardSize.y,
        DARKGRAY
    );
    DrawRectangleLines(
        (int)(ctx->zones.deck.rect.x),
        (int)(ctx->zones.deck.rect.y),
        (int)ctx->cardSize.x,
        (int)ctx->cardSize.y,
        BLACK
    );

    for (int i = 0; i < ctx->hand.count; ++i) {
        DrawCard(&ctx->hand.cards[i]);
    }

    for (int i = 0; i < ctx->hand.count; ++i) {
        ParticleDraw(&ctx->emit[i], &ctx->hand.cards[i]);
    }

    EndDrawing();
}

#include "raylib.h"

#include "game/GameContext.h"
#include "input/InputSystem.h"
#include "game/GameLogic.h"
#include "render/AnimationSystem.h"
#include "render/Renderer.h"

static void ScreenConfig(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    int screenWidth = 1920;
    int screenHeight = 1080;
    InitWindow(screenWidth, screenHeight, "Card Game - Refactor");
    SetTargetFPS(60);
}

int main(void) {
    ScreenConfig();

    GameContext ctx;
    GameContext_Init(&ctx);

    while (!WindowShouldClose()) {
        if (IsWindowResized()) {
            GameContext_OnResize(&ctx);
        }

        InputSystem_Update(&ctx);
        GameLogic_Update(&ctx);
        AnimationSystem_Update(&ctx, GetFrameTime());
        Renderer_Draw(&ctx);
    }

    GameContext_Shutdown(&ctx);
    CloseWindow();
    return 0;
}

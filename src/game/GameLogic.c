#include "GameLogic.h"

void GameLogic_Update(GameContext *ctx) {
    if (!ctx) return;

    // Procesa un evento de la cola, si existe
    processNextEvent(&ctx->sm);
    const int GRID_COLS = 20;
    const int GRID_ROWS = 20;

    if (ctx->gridX < 0) ctx->gridX = 0;
    if (ctx->gridX >= GRID_COLS) ctx->gridX = GRID_COLS - 1;
    if (ctx->gridZ < 0) ctx->gridZ = 0;
    if (ctx->gridZ >= GRID_ROWS) ctx->gridZ = GRID_ROWS - 1;

    ctx->cubePosition.x = ctx->gridX;
    ctx->cubePosition.z = ctx->gridZ;
    // Aquí podrías reaccionar al estado actual y modificar ctx->deck, ctx->hand, etc.
    // De momento lo dejamos mínimo.
}

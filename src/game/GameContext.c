#include "GameContext.h"
#include <stdio.h>
static const Vector2 DEFAULT_CARD_SIZE = {100.0f, 150.0f};

void GameContext_Init(GameContext *ctx) {
 if (!ctx) return;

    ctx->cardSize = (Vector2){100, 150};

    // INICIALIZAR 3D
    ctx->camera.position = (Vector3){ -3.0f, 3.0f, -3.0f };
    ctx->camera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
    ctx->camera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    ctx->camera.fovy     = 25.0f;
    ctx->camera.projection = CAMERA_FREE;

    ctx->gridX = 0;
    ctx->gridZ = 0;
    ctx->cubePosition = (Vector3){0, 0, 0};
    if (!ctx) return;

    ctx->cardSize = DEFAULT_CARD_SIZE;

    initStateMachine(&ctx->sm);

    Zones_Layout(&ctx->zones, ctx->cardSize);

    printf("Zones_Layout init Working\n");

    InitStandardDeck(&ctx->deck, ctx->cardSize);
    ShuffleDeck(&ctx->deck);
 
    printf("InitDeck and Shuffle Init Working\n");

    HandInit(&ctx->hand);
    for (int i = 0; i < 7; ++i) {
        Card c;
        if (DeckDrawOne(&ctx->deck, &c)) {
            HandAdd(&ctx->hand, c);
        }
    }

    printf("Hand_Init Working\n");

    LayoutHandRow(&ctx->hand, ctx->zones.hand.rect, ctx->cardSize, 20.0f);

    for (int i = 0; i < MAX_HAND; ++i) {
        InitParticles(&ctx->emit[i]);
    }

    printf("Init Particles Working\n");
    ctx->dragging = -1;
    ctx->dragOffset = (Vector2){0, 0};

    printf("GameContext_Init Working\n");
}

void GameContext_Shutdown(GameContext *ctx) {
    if (!ctx) return;
    shutdownStateMachine(&ctx->sm);
}

void GameContext_OnResize(GameContext *ctx) {
    if (!ctx) return;

    Zones_Layout(&ctx->zones, ctx->cardSize);
    LayoutHandRow(&ctx->hand, ctx->zones.hand.rect, ctx->cardSize, 20.0f);
}

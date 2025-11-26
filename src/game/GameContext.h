#ifndef GAME_CONTEXT_H
#define GAME_CONTEXT_H

#include "raylib.h"
#include "../state/state_machine.h"
#include "../objects/card.h"
#include "../objects/zone.h"
#include "../objects/card_animation.h"

typedef struct GameContext {
    StateMachine sm;
    Zones zones;
    Deck deck;
    Hand hand;

    ParticleEmitter emit[MAX_HAND];

    int dragging;
    Vector2 dragOffset;

    Vector2 cardSize;


    //3D GameInit
    Camera3D camera;
    Vector3 cubePosition;
    int gridX, gridZ;

} GameContext;

void GameContext_Init(GameContext *ctx);
void GameContext_Shutdown(GameContext *ctx);
void GameContext_OnResize(GameContext *ctx);

#endif

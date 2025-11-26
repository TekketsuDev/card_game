#include "InputSystem.h"
#include "raylib.h"
#include <string.h>
void InputSystem_Update(GameContext *ctx) {
    if (!ctx) return;

    Vector2 m = GetMousePosition();
    Zones *z = &ctx->zones;

    //Check Zones Method
    
    Vector2 mPos = GetMousePosition();
    const Zone *hovered = Zones_GetZoneAtPoint(&ctx->zones, mPos);

    // Zoom de cámara cuando el ratón está sobre la zona PLAY
    if (hovered && strcmp(hovered->name, "PLAY") == 0) {
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            ctx->camera.fovy += 10.0f;
        }
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            ctx->camera.fovy -= 10.0f;
        }
    }

    // Pick
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        for (int i = ctx->hand.count - 1; i >= 0; --i) {
            Rectangle r = CardRect(&ctx->hand.cards[i]);
            if (CheckCollisionPointRec(m, r)) {
                ctx->dragging = i;
                ctx->dragOffset = (Vector2){
                    m.x - ctx->hand.cards[i].position.x,
                    m.y - ctx->hand.cards[i].position.y
                };
                break;
            }
        }
    }

    // Drag
    if (ctx->dragging != -1 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        Card *c = &ctx->hand.cards[ctx->dragging];
        c->position = (Vector2){
            m.x - ctx->dragOffset.x,
            m.y - ctx->dragOffset.y
        };
    }

    // Drop
    if (ctx->dragging != -1 && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        int idx = ctx->dragging;
        ctx->dragging = -1;

        if (CardInZone(&ctx->hand.cards[idx], &z->play)) {
            // Lógica de "carta jugada": delegada a la lógica de juego mediante un evento
            pushEvent(&ctx->sm, makeEvent(EVENT_PLACE_CARDS));
        } else {
            // Volver la carta a la zona de mano (clamp)
            ClampCardToZone(&ctx->hand.cards[idx], &z->hand);
        }
    }

}

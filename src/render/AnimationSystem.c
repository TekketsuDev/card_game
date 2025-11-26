#include "AnimationSystem.h"

void AnimationSystem_Update(GameContext *ctx, float dt) {
    if (!ctx) return;

    for (int i = 0; i < ctx->hand.count; ++i) {
        bool inPlay = CardInZone(&ctx->hand.cards[i], &ctx->zones.play);
        int spawns = inPlay ? 3 : 1;

        for (int k = 0; k < spawns; ++k) {
            CreateParticleAtObject(&ctx->emit[i], &ctx->hand.cards[i]);
        }

        ParticleUpdate(&ctx->emit[i], dt);
    }
}

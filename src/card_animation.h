#ifndef ANIMATION_H_
#define ANIMATION_H_

#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "card.h"

#ifndef MAX_PARTICLE_HAND
#define MAX_PARTICLE_HAND 430
#endif

enum TypeParticle {
    FIRE_PARTICLE, //red 
    HEALTH_PARTICLE, //green
    DRAW_PARTICLE, //purple
    RECYCLE_PARTICLE //black grey
};

typedef struct Particle {

    Vector2 acceleration;
    Vector2 position;
    Vector2 velocity;
    float size;
    float life, lifeMax;
    int alive;
    Color color;
    enum TypeParticle typeParticle;

} Particle;

typedef struct ParticleEmitter {
    Particle p[MAX_PARTICLE_HAND];
    int count;
} ParticleEmitter;

static inline float frand(float a, float b) {
    return a + (float)rand() / (float)RAND_MAX * (b - a);
}

static void InitParticles (ParticleEmitter *pe) {
    pe->count = 0;
}
/*
static inline void CreateParticleAtObject (ParticleEmitter *pe, const Object *object) {
    
    if (pe->count >= MAX_PARTICLE_HAND) {  return; }
    int edge = rand() % 4;

    // Attack is going to happen modifiy the edge to the top of the card
    // if ( triggerAtack) { edge = 0;}
    Vector2 relativePosition = object->position;
    Vector2 relativeSize = object->size;

    Vector2 initPosition, initVelocity;
    
    switch (edge) {
        case 0:
            //borde superior
            initPosition.x = frand(relativePosition.x + 2, relativePosition.x + relativeSize.x - 2);
            initPosition.y = relativePosition.y - frand(0,8);
            initVelocity.x = frand(-10,10) * 0.02f;
            initVelocity.y = frand( -35, -20) * 0.02f;
            break;
        case 1:
            //borde inferior
            initPosition.x = frand(relativePosition.x + 2, relativePosition.x + relativeSize.x - 2);
            initPosition.y = relativePosition.y + relativeSize.y + frand(0,8);
            initVelocity.x = frand(-10,10) * 0.02f;
            initVelocity.y = frand( 20, 35) * 0.02f;
            break;
        case 2:
            //borde izquierdo
            initPosition.x = relativePosition.x - frand(0, 8);
            initPosition.y = frand(relativePosition.y + 2, relativeSize.y + relativePosition.y - 2);
            initVelocity.x = frand(-35,-20) * 0.02f;
            initVelocity.y = frand( -10, 10) * 0.02f;
            break;
        
        default:
            //borde derecho
            initPosition.x = relativePosition.x + relativeSize.x - frand(0, 8);
            initPosition.y = frand(relativePosition.y + 2, relativeSize.y + relativePosition.y - 2);
            initVelocity.x = frand(20,35) * 0.02f;
            initVelocity.y = frand( -10, 10) * 0.02f;
            break;
    }

    float size   = frand(3.0f, 6.0f);
    float life   = frand(0.4f, 0.8f);
    Color c      = (Color){ 255, (unsigned char)frand(120, 200), 20, 255 };

    
    Particle fireParticle = (Particle) {
        .acceleration = (Vector2){0.0f, -15.0f},
        .position = initPosition,
        .velocity = initVelocity,
        .size = size,
        .life = life,
        .lifeMax = life,
        .color = c,
        .alive = 1,
        .typeParticle = FIRE_PARTICLE
    };

    pe->p[pe->count++] = fireParticle;

}
*/
typedef struct {
    Color hot;       // color "caliente" (t=1, recién nacida)
    Color cool;      // color "frío" (t=0, muriendo)
    float alphaExp;  // curva de alpha: a = 255 * t^alphaExp
    float gamma;     // curva de color: u = t^gamma
} ParticleColorProfile;

// Tabla de perfiles (puedes ajustarlos a tu gusto)
static ParticleColorProfile gParticleProfiles[] = {
    [FIRE_PARTICLE]   = { {255,180, 20,255}, {255, 40,  8,255}, 0.6f, 2.0f },
    [HEALTH_PARTICLE] = { { 80,255,120,255}, { 20,100, 40,255}, 0.8f, 1.6f },
    [DRAW_PARTICLE]   = { {200, 80,255,255}, { 80, 20,140,255}, 0.7f, 1.8f },
    [RECYCLE_PARTICLE]= { {220,220,220,255}, { 70, 70, 70,255}, 1.0f, 1.2f },
};

static inline enum TypeParticle ChooseParticleTypeForObject(const Card *o) {
    // 1) Base por canal dominante del color de la carta
    unsigned r = o->color.r, g = o->color.g, b = o->color.b;
    enum TypeParticle tp;
    if (r >= g && r >= b)      tp = FIRE_PARTICLE;
    else if (g >= r && g >= b) tp = HEALTH_PARTICLE;
    else                       tp = DRAW_PARTICLE;

    // 2) Pequeña variación "pseudo-aleatoria" estable por carta
    //    (no cambia frame a frame) usando posición/slot como semilla
    unsigned seed = ((unsigned)(o->position.x*1315423911u) ^
                     (unsigned)(o->position.y*2654435761u) ^
                     (unsigned)(o->handSlot + 17u) ^
                     (unsigned)(r<<16 | g<<8 | b));
    if ((seed & 7u) == 0u)     tp = RECYCLE_PARTICLE;   // ~1/8 de las cartas “gris”

    return tp;
}

static inline Color ParticleColor(enum TypeParticle tp, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    ParticleColorProfile pr = gParticleProfiles[tp];

    float u = powf(t, pr.gamma);                   // curva de color
    Color  c = ColorLerp(pr.cool, pr.hot, u);      // t=1 -> hot, t=0 -> cool
    c.a = (unsigned char)(255.0f * powf(t, pr.alphaExp)); // curva de alpha
    return c;
}

static inline void CreateParticleAtObject(ParticleEmitter *pe, const Card *object) {
    if (pe->count >= MAX_PARTICLE_HAND) return;

    // --- control de densidad por llamada ---
    const int   PER_EDGE   = 6;      // cuántas por borde (sube/baja)
    const float EPS        = 2.0f;   // spawnea ligeramente DENTRO del rect
    const float JITTER     = 2.0f;   // ruido de posición sobre el borde
    const float IN_SPEED_A = 0.55f;  // velocidad base hacia el centro
    const float IN_SPEED_B = 0.95f;  //   (rango aleatorio)
    const float UP_BIAS    = -0.25f; // sesgo vertical (negativo = hacia arriba)

    // vida y tamaño más “flamey”
    const float SZ_MIN = 2.5f, SZ_MAX = 5.0f;
    const float LF_MIN = 1.0f, LF_MAX = 1.8f;

    // ajusta si no hay hueco
    int room = MAX_PARTICLE_HAND - pe->count;
    int maxPerEdge = room / 4;
    int perEdge = PER_EDGE <= maxPerEdge ? PER_EDGE : maxPerEdge;
    if (perEdge <= 0) return;

    Vector2 p = object->position;
    Vector2 s = object->size;
    Vector2 center = { p.x + s.x * 0.5f, p.y + s.y * 0.5f };

    // helper para normalizar
    #define NORM2(vx,vy) do{ float L=(float)sqrtf((vx)*(vx)+(vy)*(vy)); if(L>1e-5f){ (vx)/=L; (vy)/=L; } }while(0)

    // ---- TOP edge ----
    for (int i = 0; i < perEdge; ++i) {
        float x = frand(p.x + EPS, p.x + s.x - EPS);
        float y = p.y + EPS;                         // dentro de la carta
        Vector2 pos = { x + frand(-JITTER, JITTER), y };

        float dx = center.x - pos.x;
        float dy = center.y - pos.y;
        NORM2(dx, dy);
        float speed = frand(IN_SPEED_A, IN_SPEED_B);
        Vector2 vel = { dx*speed + frand(-0.06f, 0.06f), dy*speed + UP_BIAS + frand(-0.04f, 0.04f) };

        Particle fp = (Particle){
            .acceleration = (Vector2){ 0.0f, -12.0f }, // updraft suave
            .position     = pos,
            .velocity     = vel,
            .size         = frand(SZ_MIN, SZ_MAX),
            .life         = frand(LF_MIN, LF_MAX),
            .lifeMax      = 0,   // se setea abajo
            .alive        = 1,
            .color        = (Color){ 255, (unsigned char)frand(140, 220), 20, 255 },
            .typeParticle = ChooseParticleTypeForObject(object)
        };
        fp.lifeMax = fp.life;
        pe->p[pe->count++] = fp;
        if (pe->count >= MAX_PARTICLE_HAND) return;
    }

    // ---- BOTTOM edge ----
    for (int i = 0; i < perEdge; ++i) {
        float x = frand(p.x + EPS, p.x + s.x - EPS);
        float y = p.y + s.y - EPS;
        Vector2 pos = { x + frand(-JITTER, JITTER), y };

        float dx = center.x - pos.x;
        float dy = center.y - pos.y;
        NORM2(dx, dy);
        float speed = frand(IN_SPEED_A, IN_SPEED_B);
        Vector2 vel = { dx*speed + frand(-0.06f, 0.06f), dy*speed + UP_BIAS + frand(-0.04f, 0.04f) };

        Particle fp = (Particle){
            .acceleration = (Vector2){ 0.0f, -12.0f },
            .position     = pos,
            .velocity     = vel,
            .size         = frand(SZ_MIN, SZ_MAX),
            .life         = frand(LF_MIN, LF_MAX),
            .lifeMax      = 0,
            .alive        = 1,
            .color        = (Color){ 255, (unsigned char)frand(140, 220), 20, 255 },
            .typeParticle = ChooseParticleTypeForObject(object)
        };
        fp.lifeMax = fp.life;
        pe->p[pe->count++] = fp;
        if (pe->count >= MAX_PARTICLE_HAND) return;
    }

    // ---- LEFT edge ----
    for (int i = 0; i < perEdge; ++i) {
        float x = p.x + EPS;
        float y = frand(p.y + EPS, p.y + s.y - EPS);
        Vector2 pos = { x, y + frand(-JITTER, JITTER) };

        float dx = center.x - pos.x;
        float dy = center.y - pos.y;
        NORM2(dx, dy);
        float speed = frand(IN_SPEED_A, IN_SPEED_B);
        Vector2 vel = { dx*speed + frand(-0.06f, 0.06f), dy*speed + UP_BIAS + frand(-0.04f, 0.04f) };

        Particle fp = (Particle){
            .acceleration = (Vector2){ 0.0f, -12.0f },
            .position     = pos,
            .velocity     = vel,
            .size         = frand(SZ_MIN, SZ_MAX),
            .life         = frand(LF_MIN, LF_MAX),
            .lifeMax      = 0,
            .alive        = 1,
            .color        = (Color){ 255, (unsigned char)frand(140, 220), 20, 255 },
            .typeParticle = ChooseParticleTypeForObject(object)
        };
        fp.lifeMax = fp.life;
        pe->p[pe->count++] = fp;
        if (pe->count >= MAX_PARTICLE_HAND) return;
    }

    // ---- RIGHT edge ----
    for (int i = 0; i < perEdge; ++i) {
        float x = p.x + s.x - EPS;
        float y = frand(p.y + EPS, p.y + s.y - EPS);
        Vector2 pos = { x, y + frand(-JITTER, JITTER) };

        float dx = center.x - pos.x;
        float dy = center.y - pos.y;
        NORM2(dx, dy);
        float speed = frand(IN_SPEED_A, IN_SPEED_B);
        Vector2 vel = { dx*speed + frand(-0.06f, 0.06f), dy*speed + UP_BIAS + frand(-0.04f, 0.04f) };

        Particle fp = (Particle){
            .acceleration = (Vector2){ 0.0f, -12.0f },
            .position     = pos,
            .velocity     = vel,
            .size         = frand(SZ_MIN, SZ_MAX),
            .life         = frand(LF_MIN, LF_MAX),
            .lifeMax      = 0,
            .alive        = 1,
            .color        = (Color){ 255, (unsigned char)frand(140, 220), 20, 255 },
            .typeParticle = ChooseParticleTypeForObject(object)
        };
        fp.lifeMax = fp.life;
        pe->p[pe->count++] = fp;
        if (pe->count >= MAX_PARTICLE_HAND) return;
    }

    #undef NORM2
}

static inline void ParticleUpdate (ParticleEmitter *pe, float deltatime) {
    int write = 0;

    for (int i = 0; i < pe->count; i++) {
        
     Particle *q = &pe->p[i];
        if (!q->alive) continue;

        // gravedad ligera hacia arriba para "llama" (negativa en Y)
        q->velocity.x += q->acceleration.x * deltatime;
        q->velocity.y -= 15.0f * deltatime;
        q->position.x += q->velocity.x;
        q->position.y += q->velocity.y;

        q->life -= deltatime;
        if (q->life <= 0.0f) { q->alive = 0; continue; }

        // desvanecer alpha con la vida
        float t = q->life / q->lifeMax;
        float g = t*t;
    
        q->color = ParticleColor(q->typeParticle,t);
        /*
            q->color = (Color) {
            255,
            (unsigned char)(64 +191*g),
            (unsigned char)(16*g),
            (unsigned char)(255.0f * powf(t,0.6f)) 
        };
        */

        // compacta
        if (write != i) pe->p[write] = *q;
        write++;
    }
    pe->count = write;
}

static inline void ParticleDraw (ParticleEmitter *pe, const Card *object) {
        const float BORDER_T = 6.0f;
    BeginScissorMode (
        (int)object->position.x,
        (int)object->position.y,
        (int)object->size.x,
        (int)object->size.y
    );

     Rectangle outer = (Rectangle){
        object->position.x, object->position.y,
        object->size.x,     object->size.y
    };

    // Rectángulo interior (zona NO pintable)
    Rectangle inner = (Rectangle){
        (object->position.x + 2.2f*BORDER_T),
        object->position.y + 2.2f*BORDER_T,
        object->size.x - 4.0f*BORDER_T,
        object->size.y - 4.0f*BORDER_T
    };

    // Si el borde es más grande que la carta, evita tamaños negativos
    if (inner.width  < 0) inner.width  = 0;
    if (inner.height < 0) inner.height = 0;


    for (int i = 0; i < pe->count; i++) { 
        const Particle *q = &pe->p[i];
        /*
        DrawRectangle(
            (int)(q->position.x - q->size*0.5f),
            (int)(q->position.y - q->size*0.5f),
            (int)(q->size), 
            (int)(q->size),
            q->color
        );
        */
      Rectangle pr = (Rectangle){
            q->position.x - q->size * 0.5f,
            q->position.y - q->size * 0.5f,
            q->size, q->size
        };

        bool inOuter = CheckCollisionRecs(pr, outer);
        bool inInner = (inner.width > 0 && inner.height > 0) ? CheckCollisionRecs(pr, inner) : false;

        if (inOuter && !inInner) {
            DrawRectangle((int)pr.x, (int)pr.y, (int)pr.width, (int)pr.height, q->color);
        }
    }

    EndScissorMode();
}

#endif





#include "utilities.h"
#include "raylib.h"
#include "time.h"
#include <stdlib.h>

void ScreenConfig(void) {
   SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    int mon = 0;
    int scrW = GetMonitorWidth(mon), scrH = GetMonitorHeight(mon);
    int winW = (int)(scrW * 0.85f), winH = (int)(scrH * 0.85f);
    if (winW < 1024) winW = 1024;
    if (winH < 720)  winH = 720;
    InitWindow(winW, winH, "card game");
    SetWindowMinSize(900, 600);
    SetTargetFPS(60);
    //SearchAndSetResourceDir("resources");
    srand((unsigned)time(NULL));
}



#include <variant>
#include <iostream>
#include <cmath>
#include "raylib.h"
#include "../varray.cpp"

void DrawTextOutline(Font font, const char *text, Vector2 position, Color tint) {
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            if (i == 0 && j == 0) continue;
            Vector2 newpos = position;
            newpos.x += i;
            newpos.y += j;
            DrawTextEx(font, text, newpos, font.baseSize, 2.0f, BLACK);
        }
    }
    DrawTextEx(font, text, position, font.baseSize, 2.0f, tint);
}

extern "C" __declspec(dllexport) void run(varray &arr, std::vector<std::variant<int, double>> args, std::string &name) {
    Font font = LoadFontEx("font.ttf", 50, NULL, 0);
    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(BLACK);
            double scale = GetScreenWidth()/((double)arr.len);
            for (int i = 0; i < arr.len; i += std::max(1/scale, 1.0)) {
                double unitX = cos(i/(float)arr.len*PI*2);
                double unitY = sin(i/(float)arr.len*PI*2);
                double dist = (1+cos((PI*(arr.values[i]-i))/(arr.len*0.5)))*0.5; // from ArrayV
                int x = GetScreenWidth()/2+(int)(unitX*(dist*GetScreenHeight()/2));
                int y = GetScreenHeight()/2+(int)(unitY*(dist*GetScreenHeight()/2));
                DrawRectangle(x-1, y-1, 3, 3, WHITE);
                int value = arr.high.cur.load();
                if (value != -1 && abs(value-i)*scale <= 3) {
                    DrawRectangle(x-4, y-4, 9, 9, RED);
                }
            }
            DrawTextOutline(font, name.c_str(), Vector2{10, 10}, WHITE);
            DrawTextOutline(font, (std::to_string(arr.len)+" values").c_str(), Vector2{10, 10.0f+font.baseSize}, WHITE);
        EndDrawing();
    }
}
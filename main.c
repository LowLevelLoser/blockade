#include <raylib.h>

#include "game.h"
#include "rendering.h"
#include "logic.h"
#include <stdio.h>

int main(){
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Blockade");
    SetExitKey(0);
    SetTargetFPS(200);


    player_t red_p;
    red_p.p_counter = 0;
    red_p.p_moves = 16;

    player_t blue_p;
    blue_p.p_counter = 0;
    blue_p.p_moves = 16;
    blue_p.headX = 0;
    blue_p.headY = 0;

    player_t gray_p;
    gray_p.p_counter = 0;
    gray_p.p_moves = 16;

    game_t game = 
    {
        .board = {{EMPTY}},
        .turn = P_RED,
        .state = MENU_STATE,
        .choice = 2,
        .red_player = &red_p,
        .blue_player = &blue_p,
        .gray_player = &gray_p,
    };

    while(!WindowShouldClose()){
        RunGame(&game);
        BeginDrawing();
            ClearBackground(RAYWHITE);
            RenderGame(&game);
        EndDrawing();
    }
    
    //StopMusicStream(game.music);

    //CloseAudioDevice();

    CloseWindow();

    return 0;
}

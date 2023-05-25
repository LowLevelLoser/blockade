#include <raylib.h>
#include "game.h"
#include "rendering.h"

void RenderRunning(const game_t *game);
void RenderMenu(const game_t *game);
void RenderGrid(const game_t *game);
void RenderBoard(const game_t *game);
void RenderCredits();

void RenderGame(const game_t *game){
    switch(game->state){
        case MENU_STATE:
            RenderMenu(game);
            break;
        case CREDITS:
            RenderCredits();
            break;
        case P_RED_WON:
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RED);
            break;
        case P_BLUE_WON:
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLUE);
            break;
        case TIE_STATE:
            DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GRAY);
            break;
        case RUNNING_STATE_LOCAL:
        case RUNNING_STATE_COMP:
            RenderBoard(game);
            RenderGrid(game);
            break;
    }
}

void RenderMenu(const game_t *game){
    DrawText("BLOCKADE", 10, 5, 100, BLACK);
    DrawText("Player vs Player", 20, SCREEN_HEIGHT/2 - 40, 30, BLACK);
    DrawText("Player vs Computer", 20, SCREEN_HEIGHT/2, 30, BLACK);
    DrawText("Credits", 20, SCREEN_HEIGHT/2 + 40, 30, BLACK);
    DrawText(">", 5, SCREEN_HEIGHT/2 - 40*(game->choice%3-1), 30, GREEN);
}

void RenderGrid(const game_t *game){
    for(int i = 1; i < NUMBER_OF_CELLS; i++){
        DrawLine(i * CELL_WIDTH, 0, 
                i * CELL_WIDTH, SCREEN_HEIGHT, BLACK);
        DrawLine(0, i * CELL_HEIGHT,  
                SCREEN_WIDTH, i * CELL_HEIGHT, BLACK);
    }
}

void RenderBoard(const game_t *game){
    for (int x = 0; x < NUMBER_OF_CELLS; x++){
        for (int y = 0; y < NUMBER_OF_CELLS; y++){
            switch (game->board[x][y]){
                case P_RED:
                    DrawRectangle(x*CELL_WIDTH, y*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, RED);
                    break;
                case P_BLUE:
                    DrawRectangle(x*CELL_WIDTH, y*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, BLUE);
                    break;
                case P_GRAY:
                    DrawRectangle(x*CELL_WIDTH, y*CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, GRAY);
                    break;
            }
        }
    }
}

void RenderCredits(){
    DrawText("Credits:\nmade by eddi", 10, 5, 30, BLACK);
    DrawText("\n\ngithub.com/LowLevelLoser\nyoutube.com/@lowlevell0ser25", 10, 5, 30, BLUE);
}


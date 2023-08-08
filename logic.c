#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "logic.h"
#include "minimax.h"

void CheckMoves(game_t *game, player_t *player);
void TurnSwitching(game_t *game);
void MenuChoice(game_t *game);
void PlayRunningGame(game_t *game);
void ClickOnCell(game_t *game, int button, int row, int column);
bool IsValidCell(game_t *game, player_t *player, int row, int column);
void DeclareWinner(game_t *game);
void UpdatePiece(game_t *game, player_t *player, int row, int column);
void ResetGame(game_t *game);
void ClickUrls();

void RunGame(game_t *game){
    if (IsKeyPressed(KEY_ESCAPE)){
        ResetGame(game);
    }
    switch(game->state){
        case MENU_STATE:
            MenuChoice(game);
            break;
        case CREDITS:
            ClickUrls();
            break;
        case RUNNING_STATE_LOCAL:
        case RUNNING_STATE_COMP:
            PlayRunningGame(game);
            break;
        case P_RED_WON:
        case P_BLUE_WON:
        case TIE_STATE:
            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                ResetGame(game);
            break;
    }
}

void MenuChoice(game_t *game){
    switch(GetKeyPressed()){
        case KEY_UP:
            game->choice++;
            break;
        case KEY_DOWN:
            game->choice = abs(game->choice - 1);
            break;
        case KEY_ENTER:
            if (game->choice%3 == 2){
                game->state = RUNNING_STATE_LOCAL;
            }
            else if (game->choice%3 == 1){
                game->state = RUNNING_STATE_COMP;
            }
            else {
                game->state = CREDITS;
            }
            break;
    }
}

void CheckMoves(game_t *game, player_t *player){
    if (player->p_counter == 0){
        player->p_moves = NUMBER_OF_CELLS*NUMBER_OF_CELLS - game->gray_player->p_counter - game->red_player->p_counter - game->blue_player->p_counter;
        return;
    }
    else {
        player->p_moves = 0;
    }
    if (player == game->gray_player && game->gray_player->p_counter >= P_GRAY_LIMIT){
        game->gray_player->p_moves = 0;
        return;
    }
 
    for (int i = -1; i < 2; i++){
        for (int j = -1; j < 2; j++){
            if (((player->headX+j >= NUMBER_OF_CELLS)||(player->headY+i >= NUMBER_OF_CELLS))||
    			((player->headX+j < 0)||(player->headY+i < 0))){
                continue;
            }
    		if(game->board[player->headX+j][player->headY+i] == EMPTY)
            {
                player->p_moves++;
            }
        }
    }
}

void PlayRunningGame(game_t *game){
    if (game->state == RUNNING_STATE_COMP && game->turn == P_BLUE){
       RunAi(game, game->red_player, game->blue_player, game->gray_player);
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        int row = GetMouseY()/CELL_HEIGHT;
        int column = GetMouseX()/CELL_WIDTH;
        ClickOnCell(game, INSERT_PLAYER_INPUT, row, column);
        if(game->red_player->p_moves+game->blue_player->p_moves+game->gray_player->p_moves == 0)
            DeclareWinner(game);
    }
    else if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
        int row = GetMouseY()/CELL_HEIGHT;
        int column = GetMouseX()/CELL_WIDTH;
        ClickOnCell(game, INSERT_GRAY, row, column);
    }
    TurnSwitching(game);
}

void ClickOnCell(game_t *game, int button, int row, int column){
    TurnSwitching(game);
    if (button == INSERT_GRAY && IsValidCell(game, game->gray_player, row, column)){
        game->board[column][row] = P_GRAY;
        if(game->turn == P_BLUE){
            UpdatePiece(game, game->gray_player, row, column);
            game->turn = P_RED;
        }
        else{
            UpdatePiece(game, game->gray_player, row, column);
            game->turn = P_BLUE;
        }
    }
    if (button != INSERT_GRAY){
        switch(game->turn){
            case P_RED:
                if (!IsValidCell(game, game->red_player, row, column))
                    break;
                game->board[column][row] = P_RED;
                UpdatePiece(game, game->red_player, row, column);
                game->turn = P_BLUE;
                break;
            case P_BLUE:
                if (!IsValidCell(game, game->blue_player, row, column))
                    break;
                game->board[column][row] = P_BLUE;
                UpdatePiece(game, game->blue_player, row, column);
                game->turn = P_RED;
                break;
        }
    }
}

void UpdatePiece(game_t *game, player_t *player, int row, int column){
    player->headX = column;
    player->headY = row;
    player->p_counter++;
}

bool IsValidCell(game_t *game, player_t *player, int row, int column){
    CheckMoves(game, player);

    if (player->p_moves == 0)
        return false;
    if (game->board[column][row] != EMPTY)
        return false;
    if (player->p_counter == 0)
        return true;
    if (abs(row - player->headY) > 1 || abs(column - player->headX) > 1 )
        return false;
    
    return true;
}

void TurnSwitching(game_t *game){
    CheckMoves(game, game->red_player);
    CheckMoves(game, game->blue_player);
    CheckMoves(game, game->gray_player);
    if(game->red_player->p_moves+game->blue_player->p_moves+game->gray_player->p_moves <= 0)
        DeclareWinner(game);

    if (game->turn == P_RED && game->red_player->p_moves <= 0){
        if(game->blue_player->p_counter > game->red_player->p_counter){
            game->state = P_BLUE_WON;
            return;
        }
        if(game->gray_player->p_moves <= 0)
            game->turn = P_BLUE;
    }

    if (game->turn == P_BLUE && game->blue_player->p_moves <= 0){
        if (game->red_player->p_counter > game->blue_player->p_counter){
            game->state = P_RED_WON;
            return;
        }
        if(game->gray_player->p_moves <= 0)
            game->turn = P_RED;
    }
}

void DeclareWinner(game_t *game){
    if(game->red_player->p_counter == game->blue_player->p_counter){
        game->state = TIE_STATE;
        return;
    }
    else if(game->red_player->p_counter > game->blue_player->p_counter){
        game->state = P_RED_WON;
        return;
    }
    else
        game->state = P_BLUE_WON;
}

void ResetGame(game_t *game){
    game->red_player->p_counter = 0;
    game->blue_player->p_counter = 0;
    game->gray_player->p_counter = 0;
 
    memset(game->board, EMPTY, sizeof(int)*NUMBER_OF_CELLS*NUMBER_OF_CELLS);
    game->turn = P_RED;
    game->choice = 2;
    game->state = MENU_STATE;
}

void ClickUrls(){
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if (GetMouseY() > 95 && GetMouseY() <= 140){
            OpenURL("https://github.com/LowLevelLoser");
        }
        else if (GetMouseY() > 140 && GetMouseY() < 175){
            OpenURL("https://www.youtube.com/@lowlevell0ser25");
        }
    }
}

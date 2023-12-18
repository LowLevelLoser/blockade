#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include "logic.h"
#include "game.h"

// global variables start
game_t *originalGame;
int actionHistory[NUMBER_OF_CELLS*NUMBER_OF_CELLS*2][3];
int latestAction = 0;
int actionsListSim[32][3];
int numberOfActionsSim = 0;
int isDoubled = 0;
// global variables end

int PopulateActions(game_t *game, int *actions_list);
double Minimax(game_t *game, double alpha, double beta, int depth);
double PositionEvaluator(game_t *game);
void PlayActions(game_t *game, int *action);
void PlayActionsNoHistory(game_t *game, int *action);
void UndoTurn(game_t *game);

void RunAi(game_t *game, player_t *red, player_t *blue, player_t *gray){
    isDoubled = 0;
    latestAction = 0;
    game->state = THINKING;
    originalGame = game;

    game_t game_cache = *game;
    player_t red_cache = *game->red_player;
    player_t blue_cache = *game->blue_player;
    player_t gray_cache = *game->gray_player;

    game_cache.red_player = &red_cache;
    game_cache.blue_player = &blue_cache;
    game_cache.gray_player = &gray_cache;

    int actions_list [NUMBER_OF_CELLS*NUMBER_OF_CELLS*2][3];
    double actions_value [NUMBER_OF_CELLS*NUMBER_OF_CELLS*2];
    int number_of_actions = 0;
    int smallest_value_index = 0;

    number_of_actions = PopulateActions(&game_cache, actions_list[0]);

    for(int i = 0; i < number_of_actions; i++){
        PlayActions(&game_cache, actions_list[i]);
        actions_value[i] = Minimax(&game_cache, -888, 888, 5);
        UndoTurn(&game_cache);
    }

    for(int i = 0; i < number_of_actions; i++){
        if (actions_value[i] < actions_value[smallest_value_index])
            smallest_value_index = i;
    }  

    game->state = RUNNING_STATE_COMP;
    PlayActionsNoHistory(game, actions_list[smallest_value_index]);
}

int PopulateActions(game_t *game, int *actions_list){
    int index = 0;
    player_t *player;
    for (int i = 1; i >= 0; i--)
        for (int row = 0; row < NUMBER_OF_CELLS; row++)
            for (int col = 0; col < NUMBER_OF_CELLS; col++){
                if (i == 0)
                    player = game->gray_player;
                else if (game->turn == P_RED)
                    player = game->red_player;
                else if (game->turn == P_BLUE)
                    player = game->blue_player;
                if (IsValidCell(game, player, row, col)){
                    *(actions_list +3*index) = i;
                    *(actions_list +3*index + 1) = row;
                    *(actions_list +3*index + 2) = col;
                    index++;
                }
            }
    return index;
}

void PlayActions(game_t *game, int *action){
    int input = COMPUTER_INPUT;
    if(*action == 0)
        input = INSERT_GRAY;
   
    actionHistory[latestAction][0] = *(action);
    actionHistory[latestAction][1] = *(action + 1);
    actionHistory[latestAction][2] = *(action + 2);
    latestAction++;

    ClickOnCell(game, input, *(action + 1), *(action + 2));
}

void PlayActionsNoHistory(game_t *game, int *action){
    int input = COMPUTER_INPUT;
    if(*action == 0)
        input = INSERT_GRAY;

    ClickOnCell(game, input, *(action + 1), *(action + 2));
}

void UndoTurn(game_t *game){
    latestAction--;
    player_t *red_p = game->red_player;
    player_t *blue_p = game->blue_player;
    player_t *gray_p = game->gray_player;

    game_t game_c = *originalGame;
    player_t red_p_c = *originalGame->red_player;
    player_t blue_p_c = *originalGame->blue_player;
    player_t gray_p_c = *originalGame->gray_player;
    game_c.red_player = &red_p_c;
    game_c.blue_player = &blue_p_c;
    game_c.gray_player = &gray_p_c;
    
    for (int i = 0; i < latestAction; i++){
        PlayActionsNoHistory(&game_c, actionHistory[i]);
    }

    *game = game_c;
    *red_p = *game_c.red_player;
    *blue_p = *game_c.blue_player;
    *gray_p = *game_c.gray_player;
    game->red_player = red_p;
    game->blue_player = blue_p;
    game->gray_player = gray_p;
}

double PositionEvaluator(game_t *game){
    double turns = game->red_player->p_counter + game->blue_player->p_counter + game->gray_player->p_counter;

    double count_comparison = game->red_player->p_counter - game->blue_player->p_counter;
    double move_comparison = (game->red_player->p_moves - game->blue_player->p_moves);
    if (game->red_player->p_moves > 8 || game->blue_player->p_moves > 8)
        move_comparison = 0;

    return count_comparison + move_comparison/turns;
}

double Max(double a, double b){
    if (a > b)
        return a;
    return b;
}
double Min(double a, double b){
    if (a < b)
        return a;
    return b;
}

double Minimax(game_t *game_C, double alpha, double beta, int depth){
    switch(game_C->state){
        case P_BLUE_WON:
            return -99;
        case P_RED_WON:
            return 99;
        case TIE_STATE:
            return 0;
    }

    if (depth == 0){
        return PositionEvaluator(game_C);
    }

    numberOfActionsSim = PopulateActions(game_C, actionsListSim[0]);

    if (game_C->turn == P_RED){
        for(int i = 0; i <= numberOfActionsSim; i++){
            PlayActions(game_C, (actionsListSim[i]));
            double value = Minimax(game_C, alpha, beta, depth - 1);
            UndoTurn(game_C);
            numberOfActionsSim = PopulateActions(game_C, actionsListSim[0]);

            alpha = Max(alpha, value);

            if (alpha >= beta){
                break;
            }
        }
        return alpha;
    }
    else if(game_C->turn == P_BLUE){
        for(int i = 0; i <= numberOfActionsSim; i++){
            PlayActions(game_C, (actionsListSim[i]));
            double value = Minimax(game_C, alpha, beta, depth - 1);
            UndoTurn(game_C);
            numberOfActionsSim = PopulateActions(game_C, actionsListSim[0]);

            beta = Min(beta, value);

            if (alpha >= beta){
                break;
            }
        }
        return beta;
    }
    return -100000000;
}

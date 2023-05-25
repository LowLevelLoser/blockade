#ifndef GAME_H_
#define GAME_H_

//number of rows change at will personally I think 4 is the best and the higher you go the worse it gets
#define NUMBER_OF_CELLS 4

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700
#define CELL_WIDTH (SCREEN_WIDTH / NUMBER_OF_CELLS)
#define CELL_HEIGHT (SCREEN_HEIGHT / NUMBER_OF_CELLS)

#define EMPTY 0
#define P_RED 1
#define P_BLUE 2
#define P_GRAY 4

#define COMPUTER_INPUT -1
#define INSERT_GRAY 0
#define INSERT_PLAYER_INPUT 1

#define P_GRAY_LIMIT NUMBER_OF_CELLS

#define RUNNING_STATE_LOCAL -1
#define RUNNING_STATE_COMP -2
#define CREDITS -3
#define P_RED_WON 1
#define P_BLUE_WON 2
#define TIE_STATE 3
#define THINKING 4
#define MENU_STATE 0

typedef struct {
    int headX;
    int headY;
    int p_counter;
    int p_moves;
} player_t;

typedef struct {
	int board[NUMBER_OF_CELLS][NUMBER_OF_CELLS];
	int turn;
    int state;
    int choice;
    player_t *red_player;
    player_t *blue_player;
    player_t *gray_player;
} game_t;

#endif

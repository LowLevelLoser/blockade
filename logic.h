#ifndef  LOGIC_H_
#define LOGIC_H_

#include "game.h"

void RunGame(game_t *game);
void ClickOnCell(game_t *game, int button, int row, int column);
bool IsValidCell(game_t *game, player_t *player, int row, int column);

#endif // !DEBUG

#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG

#include <windows.h>
#include "game.h"
#include "ui.h"

int main(int argc, char *argv[]) {
	UI* ui = new UI();
	Game* game = new Game();

	while (ui->quit == false) {
		ui->update(game);

		if (game->isWinState(game->board) || game->isDrawState(game->board)) {
			ui->processHumanInput(game);
		}
		else

			if (game->current_player == Game::HUMAN_PLAYER) { //HUMAN
				ui->processHumanInput(game);
			}
			else { //CPU
				game->resetLastTurnStats();
				std::vector<int> emptySpaces = game->getEmptyIndexes(game->board);

				if (emptySpaces.size() == 9 && game->ifCpuFirstRandomizeFirstMove) {
					game->board[rand() % 9] = Game::CPU_PLAYER;
					ui->addMessageLog("RND FIRST MOVE");
				}
				else
				{
					LARGE_INTEGER frequency;        // ticks per second
					LARGE_INTEGER t1, t2;           // ticks
					double elapsedTime;

					// get ticks per second
					QueryPerformanceFrequency(&frequency);

					// start timer
					QueryPerformanceCounter(&t1);

					Move m;

					if (game->mode == alphabetapruning) {
						//Alpha-Beta Pruning
						m = game->alphabeta(game, game->board, 0, INT_MIN, INT_MAX, TRUE);
					}
					else {
						//MiniMax
						m = game->miniMax(game, game->board, 0, true);
					}

					game->update(m.boardIndex);

					// stop timer
					QueryPerformanceCounter(&t2);

					// compute and print the elapsed time in millisec
					elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;

					ui->addMessageLog(elapsedTime, game->terminals, game->explored);
				}
			}

			ui->update(game);
	}

	delete game;
	delete ui;

	endwin();
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}
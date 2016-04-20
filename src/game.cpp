#include <stdlib.h>     /* srand, rand */
#include <time.h>		/* time */
#include <algorithm>
#include "game.h"

Game::Game() {
	srand(time(NULL));

	//Default OPTIONS
	cpuFirst = true;
	ifCpuFirstRandomizeFirstMove = false;
	useDepth = true;
	mode = alphabetapruning;

	reset();
}

Game::~Game() {}

void Game::reset() {
	for (size_t i = 0; i < 9; i++) {
		board[i] = Game::EMPTY;
	}

	winnerPlayer = Game::EMPTY;

	if (cpuFirst) {
		current_player = CPU_PLAYER;
	}
	else {
		current_player = HUMAN_PLAYER;
	}

	resetLastTurnStats();
}

void Game::resetLastTurnStats() {
	explored = 0;
	terminals = 0;
}

bool Game::isCellEmpty(int boardIndex) {
	if (boardIndex < 0 && boardIndex > 8) return false;

	return board[boardIndex] == Game::EMPTY ? true : false;
}

void Game::update(int boardIndex) {
	if (isWinState(board)) return;

	board[boardIndex] = current_player;
	current_player = current_player == CPU_PLAYER ? HUMAN_PLAYER : CPU_PLAYER;
}

bool Game::isDrawState(const char board[]) {
	bool b1 = getEmptyIndexes(board).size() == 0;
	bool b2 = isWinState(board);

	if (b1 && b2 == false) {
		return true;
	}

	return false;
}

bool Game::isWinState(const char board[]) {
	//ROWS
	for (int i = 0; i < 9; i += 3) {
		if (board[0 + i] == board[1 + i] && board[1 + i] == board[2 + i] && board[0 + i] != Game::EMPTY) return true;
	}

	//COLUMNS
	for (int i = 0; i < 3; i += 1) {
		if (board[0 + i] == board[3 + i] && board[3 + i] == board[6 + i] && board[0 + i] != Game::EMPTY) return true;
	}

	//DIAGONALS
	if (board[0] == board[4] && board[4] == board[8] && board[0] != Game::EMPTY) return true;
	if (board[6] == board[4] && board[4] == board[2] && board[6] != Game::EMPTY) return true;

	return false;
}

std::vector<int> Game::getEmptyIndexes(const char board[]) {
	std::vector<int> emptySpaces;

	for (size_t i = 0; i < 9; i++)
	{
		if (board[i] == Game::EMPTY) {
			emptySpaces.push_back(i);
		}
	}
	return emptySpaces;
}

Move Game::miniMax(Game* game, const char board[], int depth, bool maximizingPlayer) {
	game->explored += 1;

	if (isDrawState(board)) {
		game->terminals += 1;
		return Move{ DRAW_SCORE, 1, depth };
	}

	if (isWinState(board)) {
		game->terminals += 1;
		if (maximizingPlayer == false) {
			return Move{ WIN_SCORE, -1, depth };
		}
		else {
			return Move{ LOSE_SCORE, -1, depth };
		}
	}

	if (maximizingPlayer) {
		//MAX
		Move bestMove = Move{ INT_MIN, -1 };
		std::vector<int> emptySpaces = getEmptyIndexes(board);

		for (size_t i = 0; i < emptySpaces.size(); i++)
		{
			char newBoard[]{ board[0], board[1],board[2],board[3],board[4],board[5],board[6],board[7],board[8] };
			newBoard[emptySpaces[i]] = CPU_PLAYER;

			Move childMove = miniMax(game, newBoard, depth + 1, false);

			if (childMove.v > bestMove.v) {
				bestMove.v = childMove.v;
				bestMove.boardIndex = emptySpaces[i];
				bestMove.terminalDepth = childMove.terminalDepth;
			}
			else if (game->useDepth && (childMove.v == bestMove.v && childMove.terminalDepth < bestMove.terminalDepth)) {
				bestMove.v = childMove.v;
				bestMove.boardIndex = emptySpaces[i];
				bestMove.terminalDepth = childMove.terminalDepth;
			}
		}

		return bestMove;
	}
	else { //MIN
		Move bestMove = Move{ INT_MAX, -1 };
		std::vector<int> emptySpaces = getEmptyIndexes(board);

		for (size_t i = 0; i < emptySpaces.size(); i++) {
			char newBoard[]{ board[0], board[1],board[2],board[3],board[4],board[5],board[6],board[7],board[8] };
			newBoard[emptySpaces[i]] = HUMAN_PLAYER;

			Move childMove = miniMax(game, newBoard, depth + 1, true);

			if (childMove.v < bestMove.v) {
				bestMove.v = childMove.v;
				bestMove.boardIndex = emptySpaces[i];
				bestMove.terminalDepth = childMove.terminalDepth;
			}
			else if (game->useDepth && (childMove.v == bestMove.v && childMove.terminalDepth < bestMove.terminalDepth)) {
				bestMove.v = childMove.v;
				bestMove.boardIndex = emptySpaces[i];
				bestMove.terminalDepth = childMove.terminalDepth;
			}
		}

		return bestMove;
	}
}

Move Game::alphabeta(Game* game, const char board[], int depth, int alpha, int beta, bool maximizingPlayer) {
	game->explored += 1;

	if (isDrawState(board)) {
		game->terminals += 1;
		return Move{ DRAW_SCORE, 1, depth };
	}

	if (isWinState(board)) {
		game->terminals += 1;
		if (maximizingPlayer == false) {
			return Move{ WIN_SCORE, -1, depth };
		}
		else {
			return Move{ LOSE_SCORE, -1, depth };
		}
	}

	if (maximizingPlayer) { //maximizing player
		Move bestMove = Move{ INT_MIN, -1, INT_MAX };

		std::vector<int> emptySpaces = getEmptyIndexes(board);

		for (size_t i = 0; i < emptySpaces.size(); i++) {
			char newBoard[]{ board[0], board[1],board[2],board[3],board[4],board[5],board[6],board[7],board[8] };
			newBoard[emptySpaces[i]] = CPU_PLAYER;

			Move childMove = alphabeta(game, newBoard, depth + 1, alpha, beta, false);

			int childV = childMove.v;
			int childTerminalDepth = childMove.terminalDepth;

			if (childV > bestMove.v) { //MAX()
				bestMove.v = childV;
				bestMove.boardIndex = emptySpaces[i];
				bestMove.terminalDepth = childTerminalDepth;
			}
			else if (game->useDepth && (childV == bestMove.v && childTerminalDepth < bestMove.terminalDepth)) {
				bestMove.v = childV;
				bestMove.boardIndex = emptySpaces[i];
				bestMove.terminalDepth = childTerminalDepth;
			}

			if (bestMove.v >= beta) {
				break;
			}

			alpha = std::max(alpha, bestMove.v);
		}
		return bestMove;
	}
	else {//minimizing player
		Move bestMove = Move{ INT_MAX, -1, INT_MAX };

		std::vector<int> emptySpaces = getEmptyIndexes(board);

		for (size_t i = 0; i < emptySpaces.size(); i++) {
			char newBoard[]{ board[0], board[1],board[2],board[3],board[4],board[5],board[6],board[7],board[8] };
			newBoard[emptySpaces[i]] = HUMAN_PLAYER;

			Move childMove = alphabeta(game, newBoard, depth + 1, alpha, beta, true);

			int childV = childMove.v;
			int childTerminalDepth = childMove.terminalDepth;

			if (childV < bestMove.v) { //MIN()
				bestMove.v = childV;
				bestMove.boardIndex = emptySpaces[i];
				bestMove.terminalDepth = childTerminalDepth;
			}
			else if (game->useDepth && (childV == bestMove.v && childTerminalDepth < bestMove.terminalDepth)) {
				bestMove.v = childV;
				bestMove.boardIndex = emptySpaces[i];
				bestMove.terminalDepth = childTerminalDepth;
			}

			if (bestMove.v <= alpha) {
				break;
			}

			beta = std::min(beta, bestMove.v);
		}
		return bestMove;
	}
}
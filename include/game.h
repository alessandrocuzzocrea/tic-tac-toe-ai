#ifndef GAME_H
#define GAME_H

#include <vector>

class State;

struct Move {
	int v;
	int boardIndex;
	int terminalDepth;
};

enum AiMode
{
	minimax = 0,
	alphabetapruning,
};

class Game
{
public:
	Game();
	~Game();

	char current_player;
	char winnerPlayer;

	Move alphabeta(Game* game, const char board[], int depth, int alpha, int beta, bool maximizingPlayer);
	Move miniMax(Game* game, const char board[], int depth, bool maximizingPlayer);

	void reset();
	void resetLastTurnStats();
	bool isDrawState(const char board[]);
	bool isWinState(const char board[]);
	std::vector<int> getEmptyIndexes(const char board[]);
	char board[9];

	//options
	bool cpuFirst;
	bool ifCpuFirstRandomizeFirstMove;
	bool useDepth;
	AiMode mode;

	//stats
	long explored;
	long terminals;

	void update(int boardIndex);
	bool isCellEmpty(int boardIndex);

	//consts
	static char const EMPTY = ' ';
	static char const CPU_PLAYER = 'x';
	static char const HUMAN_PLAYER = 'o';

	static int const WIN_SCORE = 1;
	static int const DRAW_SCORE = 0;
	static int const LOSE_SCORE = -1;

private:
};

#endif /* GAME_H */
#ifndef UI_H
#define UI_H

#include <vector>
#include <curses.h>
#include "game.h"

class UI
{
public:
	UI();
	~UI();

	void printPlayerChar(WINDOW* win, char p);
	void printCurrentPlayer(WINDOW* win, char p);
	void printGame(Game* game, WINDOW* win);
	void addMessageLog(double elapsedTime, long terminals, long explored);
	void addMessageLog(std::string s);
	void resetLog();

	//Draw game windows
	void drawGameWin(Game* game);
	void drawLogWin(Game* game);
	void drawOptionWin(Game* game);
	void update(Game* game);

	//Input
	int boardIndexFromKeypad(char key);
	void processHumanInput(Game* game);

	bool quit;

private:
	WINDOW* gameWin;
	WINDOW* optionsWin;
	WINDOW* logWin;

	std::vector<std::string> messageLog;
};

#endif /* UI_H */
#include <string>
#include "ui.h"

UI::UI() {
	initscr();
	raw();
	noecho();
	start_color();

	curs_set(0);

	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);

	gameWin = newwin(9, 11, 4, 8);
	optionsWin = newwin(20, 30, 1, 30);
	logWin = newwin(8, 60, 15, 1);

	quit = false;
}

UI::~UI() {}

void UI::printPlayerChar(WINDOW* win, char p) {
	if (p == Game::EMPTY) {
		waddch(win, ' ');
		return;
	}

	int curr_player_color = p == Game::CPU_PLAYER ? 1 : 2;
	wattron(win, COLOR_PAIR(curr_player_color));
	waddch(win, p);
	wattroff(win, COLOR_PAIR(curr_player_color));
}

void UI::printCurrentPlayer(WINDOW* win, char p) {
	wmove(win, 1, 0);
	wprintw(win, "Current Player: ");
	printPlayerChar(win, p);
	refresh();
}

void UI::printGame(Game* game, WINDOW* win) {
	char c0 = game->board[0];
	char c1 = game->board[1];
	char c2 = game->board[2];
	char c3 = game->board[3];
	char c4 = game->board[4];
	char c5 = game->board[5];
	char c6 = game->board[6];
	char c7 = game->board[7];
	char c8 = game->board[8];

	wmove(win, 2, 3); printPlayerChar(win, c0); waddch(win, '|'); printPlayerChar(win, c1); waddch(win, '|'); printPlayerChar(win, c2);
	wmove(win, 3, 3); waddch(win, '-'); waddch(win, '-'); waddch(win, '-'); waddch(win, '-'); waddch(win, '-');
	wmove(win, 4, 3); printPlayerChar(win, c3); waddch(win, '|'); printPlayerChar(win, c4); waddch(win, '|'); printPlayerChar(win, c5);
	wmove(win, 5, 3); waddch(win, '-'); waddch(win, '-'); waddch(win, '-'); waddch(win, '-'); waddch(win, '-'); waddch(win, '\n');
	wmove(win, 6, 3); printPlayerChar(win, c6); waddch(win, '|'); printPlayerChar(win, c7); waddch(win, '|'); printPlayerChar(win, c8);
}

void UI::addMessageLog(double elapsedTime, long terminals, long explored) {
	messageLog.push_back(std::to_string(elapsedTime) + " msecs; " + std::to_string(terminals) + "/" + std::to_string(explored));
}

void UI::addMessageLog(std::string s) {
	messageLog.push_back(s);
}

void UI::resetLog() {
	messageLog.clear();
}

void UI::drawGameWin(Game* game) {
	box(gameWin, '*', '*');
	mvwprintw(gameWin, 0, 1, "Game");
	printGame(game, gameWin);

	if (game->isWinState(game->board) || game->isDrawState(game->board)) {
		wmove(stdscr, 1, 0);
		if (game->isWinState(game->board)) printw("CPU WINS");
		if (game->isDrawState(game->board)) printw("DRAW");
		wmove(stdscr, 2, 0);
		printw("Play Again? (y/n)");
		refresh();
	}
	else {
		printCurrentPlayer(stdscr, game->current_player);
	}

	wrefresh(gameWin);
}

void UI::drawLogWin(Game* game) {
	box(logWin, '*', '*');
	mvwprintw(logWin, 0, 1, "LOG");

	for (size_t i = 0; i < messageLog.size(); i++) {
		mvwprintw(logWin, 1 + i, 1, ("Cpu Move #" + std::to_string(messageLog.size() - i) + ": " + messageLog[messageLog.size() - 1 - i]).c_str());
	}

	wrefresh(logWin);
}

void UI::drawOptionWin(Game* game) {
	box(optionsWin, '*', '*');

	mvwprintw(optionsWin, 0, 1, "OPTIONS");

	//cpuFirst;
	mvwprintw(optionsWin, 1, 1, game->cpuFirst ? "[q] X" : "[q] _");
	wprintw(optionsWin, " cpuFirst");

	//ifCpuFirstRandomizeFirstMove;
	mvwprintw(optionsWin, 2, 1, game->ifCpuFirstRandomizeFirstMove ? "[w] X" : "[w] _");
	wprintw(optionsWin, " ifCpuFirstRndFirstMv");

	//useDepth;
	mvwprintw(optionsWin, 3, 1, game->useDepth ? "[e] X" : "[e] _");
	wprintw(optionsWin, " useDepth");

	//minimax;
	mvwprintw(optionsWin, 5, 1, game->mode == minimax ? "[a] X" : "[a] _");
	wprintw(optionsWin, " Use MiniMax");

	//useAlphaBetaPruning;
	mvwprintw(optionsWin, 6, 1, game->mode == alphabetapruning ? "[s] X" : "[s] _");
	wprintw(optionsWin, " Use AlphaBetaPruning");

	//touchwin(optionsWin);
	wrefresh(optionsWin);
}

void UI::update(Game* game) {
	clear();
	wclear(optionsWin);
	wclear(logWin);

	drawGameWin(game);
	drawOptionWin(game);
	drawLogWin(game);
}
int UI::boardIndexFromKeypad(char key) {
	if (key == '1') return 6;
	if (key == '2') return 7;
	if (key == '3') return 8;
	if (key == '4') return 3;
	if (key == '5') return 4;
	if (key == '6') return 5;
	if (key == '7') return 0;
	if (key == '8') return 1;
	if (key == '9') return 2;
}

void UI::processHumanInput(Game* game) {
	char input = getchar();

	if (input == 'q') game->cpuFirst = !game->cpuFirst;
	if (input == 'w') game->ifCpuFirstRandomizeFirstMove = !game->ifCpuFirstRandomizeFirstMove;
	if (input == 'e') game->useDepth = !game->useDepth;

	if (input == 'a') game->mode = minimax;
	if (input == 's') game->mode = alphabetapruning;

	if (input == 'y') {
		resetLog();
		game->reset();
	}

	if (input == 'n') {
		quit = true;
	}

	if (isdigit(input) && input != '0') {
		int boardIndex = boardIndexFromKeypad(input);

		if (game->isCellEmpty(boardIndex)) {
			game->update(boardIndex);
		}
	}
}
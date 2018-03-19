#include <ncurses.h>

int main()
{
	initscr();
	curs_set(0);
	printw("Hello, World");
	refresh();
	getch();
	endwin();
}

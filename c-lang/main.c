#include <stdio.h>
#include "display.h"
#include "reversi.h"

static void turn(SCREEN *d, GAME *g, int x, int y);

int main(int argc, char *argv[])
{
	GAME game;
	SCREEN scr;
	int xy = 0, x = 0, y = 0;
	// X Window open
	windowCreate(&scr);
	game = gameStart(1, 7);
	printBoard(&scr, &game);
	while(1){
		xy = getPoint(&scr);
		y = xy / MASU_SIZE;
		x = xy % MASU_SIZE;
		if(x >= 0 && x < MASU_SIZE && y >= 0 && y < MASU_SIZE){
			turn(&scr, &game, x, y);
		}
		printBoard(&scr, &game);
	}
	// X Window close
	windowClose(&scr);
	return 0;
}

static void turn(SCREEN *d, GAME *g, int x, int y)
{
	if(checkReverse(g, y, x)){
		g->all++;
		printBoard(d, g);
		aiStoneCheck(g);
	} else {
		if(boardStoneCheck(g)){
			printf("It is impossible to put the stone there.\n");
		} else {
			aiStoneCheck(g);
		}
	}
}


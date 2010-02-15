#include <stdio.h>
#include "reversi.h"

/* プロトタイプ宣言 */
static int putStoneCheck(GAME *g, int i, int j);
static int stoneCheck(const GAME *g, int i, int j, int y, int x);
static int boardPoint(GAME *g, int flag);
static int decisionStone(GAME *g);
static int decisionCount(GAME *g, int y, int x, int type);
static int AandB(GAME *g, int flag, int ai, int a, int b);
static void turnChange(GAME *g);
static void keep(GAME *g, int lv);
static void back(GAME *g, int lv);
static int hanten(GAME *g, int i, int j, int y, int x);

const static int pointmap[MASU_SIZE][MASU_SIZE] = {
	{ 30,-12,  0, -1, -1,  0,-12, 30},
	{-12,-15, -3, -3, -3, -3,-15,-12},
	{  0, -3,  0, -1, -1,  0, -3,  0},
	{ -1, -3, -1, -1, -1, -1, -3, -1},
	{ -1, -3, -1, -1, -1, -1, -3, -1},
	{  0, -3,  0, -1, -1,  0, -3,  0},
	{-12,-15, -3, -3, -3, -3,-15,-12},
	{ 30,-12,  0, -1, -1,  0,-12, 30}
};

const static char map[2][8] = {
	{ -1,  0,  1, -1,  1, -1,  0,  1},
	{ -1, -1, -1,  0,  0,  1,  1,  1}
};

const static unsigned char dmap[8][3] = {
	{ 0, 0, 1},
	{ 0, 0, 2},
	{ 0, 7, 1},
	{ 0, 7, 4},
	{ 7, 0, 3},
	{ 7, 0, 2},
	{ 7, 7, 3},
	{ 7, 7, 4}
};

GAME gameStart(int turn, int AILv)
{
	GAME game = {
		{{{ 0, 0, 0, 0, 0, 0, 0, 0},
		  { 0, 0, 0, 0, 0, 0, 0, 0},
		  { 0, 0, 0, 0, 0, 0, 0, 0},
		  { 0, 0, 0,-1, 1, 0, 0, 0},
		  { 0, 0, 0, 1,-1, 0, 0, 0},
		  { 0, 0, 0, 0, 0, 0, 0, 0},
		  { 0, 0, 0, 0, 0, 0, 0, 0},
		  { 0, 0, 0, 0, 0, 0, 0, 0}}},

		{{{{0}}}},

		turn,
		AILv,
		1
	};

	if(game.turn == MASU_KURO){
		aiStoneCheck(&game);
	}
	return game;
}

void aiStoneCheck(GAME *g)
{
	int ij = 0, x = 0, y = 0;
	turnChange(g);
	ij = AandB(g, 1, g->AILv, MINIMAMU, MAXIMAMU);
	y = ij / MASU_SIZE;
	x = ij % MASU_SIZE;
	if(y < MASU_SIZE || y >= 0 || x < MASU_SIZE || x >= 0){
		checkReverse(g, y, x);
		printf("AI put the stone on (%d,%d).\n", x, y);
		g->all++;
	} else {
		printf("AI was pass up.\n");
	}
	turnChange(g);
}

int boardStoneCheck(GAME *g)
{
	int count = 0, i = 0, j = 0;
	for(i = 0; i < MASU_SIZE; i++){
		for(j = 0; j < MASU_SIZE; j++){
			if(g->board.data[i][j] == MASU_BACK){
				if(putStoneCheck(g, i, j)){
					count++;
				}
			}
		}
	}
	return count;
}
	
int checkReverse(GAME *g, int i, int j)
{
	int count = 0, han = 0, k = 0;
	if(g->board.data[i][j] == MASU_BACK){
		for(; k < MASU_SIZE; k++){
			if(stoneCheck(g, i, j, map[0][k], map[1][k])){
				han += hanten(g, i, j, map[0][k], map[1][k]);
				count++;
			}
		}
	}
	return han;
}

static int putStoneCheck(GAME *g, int i, int j)
{
	int k = 0;
	for(; k < MASU_SIZE; k++){
		if(stoneCheck(g, i, j, map[0][k], map[1][k])){
			return -1;
		}
	}
	return 0;
}

static int stoneCheck(const GAME *g, int i, int j, int y, int x)
{
	i += y;
	j += x;
	if((i >= MASU_SIZE) || (i < 0) || (j >= MASU_SIZE) || (j < 0)){
		return 0;
	}
	if(g->board.data[i][j] != (g->turn * -1)){
		return 0;
	}
	i += y;
	j += x;
	while((i < MASU_SIZE) && (i >= 0) && (j < MASU_SIZE) && (j >= 0)){
		if(g->board.data[i][j] == MASU_BACK){
			return 0;
		}
		if(g->board.data[i][j] == g->turn){
			return -1;
		}
		i += y;
		j += x;
	}
	return 0;
}

static int boardPoint(GAME *g, int flag)
{
	int point = 0, i = 0, j = 0;
	for(i = 0; i < MASU_SIZE; i++){
		for(j = 0; j < MASU_SIZE; j++){
			point += g->board.data[i][j] * pointmap[i][j];
		}
	}
	point += decisionStone(g) * 5;
	
	if(!flag){
		point *= -1;
	}
	return point;
}

static int decisionStone(GAME *g)
{
	int point = 0, i = 0;
	for(; i < 8; i++){
		if(g->board.data[dmap[i][0]][dmap[i][1]] != MASU_SIZE){
			point += decisionCount(g, dmap[i][0], dmap[i][1], dmap[i][2]);
		}
	}
	return point;
}

static int decisionCount(GAME *g, int y, int x, int type)
{
	int count = 0;
	int my = 0;
	int my2 = 0;
	int enemy = 0;
	int enemy2 = 0;
	
	while(g->board.data[y][x] != MASU_SIZE){
		if((g->board.data[y][x] == g->turn) && (my == count)){
			my++;
		} else if(g->board.data[y][x] == g->turn){
			my2++;
		}
		if((g->board.data[y][x] != g->turn) && (enemy == count)){
			enemy++;
		} else if(g->board.data[y][x] != g->turn){
			enemy2++;
		}
		if(count >= 7){
			return ((my + my2) - (enemy + enemy2));
		}
		count++;
		switch(type){
		case 1:
			y++;
			break;
		case 2:
			x++;
			break;
		case 3:
			y--;
			break;
		case 4:
			x--;
			break;
		default:
			break;
		}
	}
	return (my - enemy);
}

static int AandB(GAME *g, int flag, int ai, int a, int b)
{
	int val, Cval, bestY = -1, bestX = -1;
	int i = 0, j = 0;
	if(ai == 0) return boardPoint(g, flag);
	
	if(flag){
		val = MINIMAMU;
	} else {
		val = MAXIMAMU;
	}
	keep(g, ai);
	for(i = 0; i < MASU_SIZE; i++){
		for(j = 0; j < MASU_SIZE; j++){
			if(g->board.data[i][j] == MASU_BACK){
				if(putStoneCheck(g, i, j)){
					checkReverse(g, i, j);
					turnChange(g);
					Cval = AandB(g, !flag, ai - 1, a, b);
					turnChange(g);
					back(g, ai);
					if(flag){
						if(Cval > val){
							val = Cval;
							a = val;
							bestY = i;
							bestX = j;
						}
						if(val > b){
							return val;
						}
					} else {
						if(Cval < val){
							val = Cval;
							b = val;
							bestY = i;
							bestX = j;
						}
						if(val < a){
							return val;
						}
					}
				}
			}
		}
	}
	if(ai == g->AILv){
		if((bestY < 0) || (bestX < 0)){
			return 0;
		}
		return ((bestY * MASU_SIZE) + bestX);
	}
	if((bestY < 0) || (bestX < 0)){
		return boardPoint(g, flag);
	}
	return val;
}

static void turnChange(GAME *g)
{
	g->turn *= -1;
}

static void keep(GAME *g, int lv)
{
	g->backup[lv] = g->board;
}

static void back(GAME *g, int lv)
{
	g->board = g->backup[lv];
}

static int hanten(GAME *g, int i, int j, int y, int x)
{
	int han = 1;
	int turn = g->turn * -1;

	g->board.data[i][j] = g->turn;
	i += y;
	j += x;
	
	while(g->board.data[i][j] == turn){
		g->board.data[i][j] = g->turn;
		i += y;
		j += x;
		han++;
	}
	return han;
}


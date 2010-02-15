#ifndef INCLUDE_REVERSIKUN_REVERSI_H
#define INCLUDE_REVERSIKUN_REVERSI_H

/* íËêîêÈåæ */
#define MASU_SIZE		(8)
#define MASU_SIRO		(1)
#define MASU_KURO		(-1)
#define MASU_BACK		(0)
#define MAXIMAMU		(1000000)
#define MINIMAMU		(-1000000)

typedef struct {
	char data[MASU_SIZE][MASU_SIZE];
} BOARD;

typedef struct {
	BOARD board;
	BOARD backup[10];
	char turn;
	unsigned char AILv;
	unsigned char all;
} GAME;

// Function
GAME gameStart(int turn, int AILv);
int boardStoneCheck(GAME *g);
int checkReverse(GAME *g, int i, int j);
void aiStoneCheck(GAME *g);

#endif /* INCLUDE_REVERSIKUN_REVERSI_H */

#include <stdio.h>
#include "display.h"
#include "reversi.h"

/* プロトタイプ宣言 */
static int getColor(const SCREEN *d, const char *c);
static void flash(const SCREEN *d);

void windowCreate(SCREEN *d)
{
	d->d = XOpenDisplay(NULL);
	d->root_w = DefaultRootWindow(d->d);
	d->w = XCreateSimpleWindow(d->d, d->root_w, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 2, BlackPixel(d->d, 0), WhitePixel(d->d, 0));
	
	d->siro = XCreateGC(d->d, d->w, 0, 0);
	d->kuro = XCreateGC(d->d, d->w, 0, 0);
	d->back = XCreateGC(d->d, d->w, 0, 0);
	
	XMapWindow(d->d, d->w);

	XSetLineAttributes(d->d, d->siro, 1, 0, 0, 0);
	XSetForeground(d->d, d->siro, getColor(d, "white"));
	XSetLineAttributes(d->d, d->kuro, 1, 0, 0, 0);
	XSetForeground(d->d, d->kuro, getColor(d, "black"));
	XSetLineAttributes(d->d, d->back, 1, 0, 0, 0);
	XSetForeground(d->d, d->back, getColor(d, "green"));

	XSelectInput(d->d, d->w, ButtonPressMask);
}

void windowClose(const SCREEN *d)
{
	XDestroyWindow(d->d, d->w);
	XCloseDisplay(d->d);
}

int getPoint(SCREEN *d)
{
	int x = 0, y = 0;
	XNextEvent(d->d, &(d->event));
	switch(d->event.type){
		case ButtonPress:
			x = (d->event.xbutton.x - (d->event.xbutton.x % MASU_WIDTH)) / MASU_WIDTH;
			y = (d->event.xbutton.y - (d->event.xbutton.y % MASU_HEIGHT)) / MASU_HEIGHT;
			printf("Player put the stone on (%d,%d).\n", x, y);
			return ((y * MASU_SIZE) + x);
		default:
			break;
	}
	return -1;
}

void printBoard(const SCREEN *d, const GAME *g)
{
	int i = 0, j = 0, x = 0, y = 0;
	XFillRectangle(d->d, d->w, d->kuro, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	for(i = 0; i < MASU_SIZE; i++){
		for(j = 0; j < MASU_SIZE; j++){
			x = (j * MASU_WIDTH) + j;
			y = (i * MASU_HEIGHT) + i;
			XFillRectangle(d->d, d->w, d->back, x, y, MASU_WIDTH, MASU_HEIGHT);
			if(g->board.data[i][j] == MASU_SIRO){
				XFillArc(d->d, d->w, d->siro, x, y, MASU_WIDTH, MASU_HEIGHT, 0, 360*64);
			} else if(g->board.data[i][j] == MASU_KURO){
				XFillArc(d->d, d->w, d->kuro, x, y, MASU_WIDTH, MASU_HEIGHT, 0, 360*64);
			}
		}
	}
	flash(d);
}

static int getColor(const SCREEN *d, const char *c)
{
	Colormap cmap;
	XColor c0, c1;
	cmap = DefaultColormap(d->d, 0);
	XAllocNamedColor(d->d, cmap, c, &c1, &c0);
	return (int)c1.pixel;
}

static void flash(const SCREEN *d)
{
	XFlush(d->d);
}


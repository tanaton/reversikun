#ifndef INCLUDE_REVERSIKUN_DISPLAY_H
#define INCLUDE_REVERSIKUN_DISPLAY_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "reversi.h"

/* íËêîêÈåæ */
#define WINDOW_WIDTH	(320)
#define WINDOW_HEIGHT	(320)
#define MASU_WIDTH		(30)
#define MASU_HEIGHT		(30)

typedef struct {
	Display *d;
	Window w;
	Window root_w;
	XEvent event;
	XSetWindowAttributes att;
	GC siro;
	GC kuro;
	GC back;
} SCREEN;

void windowCreate(SCREEN *d);
void windowClose(const SCREEN *d);
int getPoint(SCREEN *d);
void printBoard(const SCREEN *d, const GAME *g);

#endif /* INCLUDE_REVERSIKUN_DISPLAY_H */

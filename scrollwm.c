/*******************************************************************\
* SCROLLWM - a floating WM with a single large scrollable workspace
*
* Author: Jesse McClure, copyright 2012
* License: GPLv3
*
* Based on code from TinyWM and TTWM
* TinyWM is written by Nick Welch <mack@incise.org>, 2005.
* TTWM is written by Jesse McClure, 2011-2012
\*******************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct Client Client;
struct Client {
/*
	char *title;
	int tlen;
*/
	int x, y;
	Client *next;
	Window win;
};

static void buttonpress(XEvent *);
static void buttonrelease(XEvent *);
static void keypress(XEvent *);
static void maprequest(XEvent *);
static void motionnotify(XEvent *);
static void unmapnotify(XEvent *);

static void scrollwindows(Client *,int,int);
static Client *wintoclient(Window);

static Display * dpy;
static Window root;
static XWindowAttributes attr;
static XButtonEvent start;
static Client *clients=NULL;
static Bool running = True;
static void (*handler[LASTEvent]) (XEvent *) = {
	[ButtonPress]	= buttonpress,
	[ButtonRelease]	= buttonrelease,
	[KeyPress]		= keypress,
	[MapRequest]	= maprequest,
	[MotionNotify]	= motionnotify,
	[UnmapNotify]	= unmapnotify,
};

void buttonpress(XEvent *e) {
	if (e->xbutton.button > 3) return;
	Window w;
	if(!e->xbutton.subwindow) w = root;
	else w = e->xbutton.subwindow;
	if (w==root && e->xbutton.state == Mod4Mask) return;
	if (w != root) {
		XSetInputFocus(dpy,w,RevertToPointerRoot,CurrentTime);
		XRaiseWindow(dpy,w);
	}
	XGrabPointer(dpy,w,True,PointerMotionMask | ButtonReleaseMask,GrabModeAsync,GrabModeAsync, None, None, CurrentTime);
	XGetWindowAttributes(dpy,w, &attr);
	start = e->xbutton;
}

void buttonrelease(XEvent *e) {
	XUngrabPointer(dpy, CurrentTime);
}

void keypress(XEvent *e) {
	system("urxvt &");
}

void maprequest(XEvent *e) {
	Client *c;
	static XWindowAttributes wa;
	XMapRequestEvent *ev = &e->xmaprequest;
	if (!XGetWindowAttributes(dpy, ev->window, &wa)) return;
	if (wa.override_redirect) return;
	if (!wintoclient(ev->window)) {
		if (!(c=calloc(1,sizeof(Client)))) exit(1);
		c->win = ev->window;
		XGetWindowAttributes(dpy,c->win, &attr);
		c->x = attr.x; c->y = attr.y;
	//	if (XFetchName(dpy,c->win,&c->title)) c->tlen = strlen(c->title);
	//	XSelectInput(dpy,c->win,PropertyChangeMask);
		c->next = clients;
		clients = c;
		XMapWindow(dpy,c->win);
	}
}

void motionnotify(XEvent *e) {
	int xdiff, ydiff;
	while(XCheckTypedEvent(dpy,MotionNotify,e));
	Client *c = wintoclient(e->xbutton.window);
	xdiff = e->xbutton.x_root - start.x_root;
	ydiff = e->xbutton.y_root - start.y_root;
	if (start.button == 1 && start.state == Mod4Mask)
		XMoveWindow(dpy,c->win,(c->x=attr.x+xdiff),(c->y=attr.y+ydiff));
	else if (start.button == 3 && start.state == Mod4Mask)
		XResizeWindow(dpy,c->win,attr.width+xdiff,attr.height+ydiff);
	else if (start.button == 1 && start.state == Mod1Mask | Mod4Mask) {
		scrollwindows(clients,xdiff,ydiff);
		start.x_root+=xdiff; start.y_root+=ydiff;
	}
}

void unmapnotify(XEvent *e) {
	Client *c,*t;
	XUnmapEvent *ev = &e->xunmap;
	if (!(c=wintoclient(ev->window))) return;
	if (!ev->send_event) {
		if (c == clients) clients = c->next;
		else {
			for (t = clients; t && t->next != c; t = t->next);
			t->next = c->next;
		}
		//XFree(c->title);
		free(c);
	}
}

void scrollwindows(Client *stack, int x, int y) {
	while (stack) {
		XGetWindowAttributes(dpy,stack->win, &attr);
		XMoveWindow(dpy,stack->win,(stack->x=attr.x+x),(stack->y=attr.y+y));
		stack = stack->next;
	}
}

Client *wintoclient(Window w) {
	Client *c;
	for (c = clients; c && c->win != w; c = c->next);
	if (c) return c;
	else return NULL;
}

int main() {
    if(!(dpy = XOpenDisplay(0x0))) return 1;
    root = DefaultRootWindow(dpy);
	XSetWindowAttributes wa;
	wa.event_mask = ExposureMask | FocusChangeMask | SubstructureNotifyMask |
					ButtonReleaseMask | PropertyChangeMask | SubstructureRedirectMask | StructureNotifyMask;
	XChangeWindowAttributes(dpy,root,CWEventMask,&wa);
	XSelectInput(dpy,root,wa.event_mask);
    XGrabKey(dpy, XKeysymToKeycode(dpy, XStringToKeysym("F1")), Mod1Mask, root,
            True, GrabModeAsync, GrabModeAsync);
    XGrabButton(dpy, AnyButton, Mod4Mask, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
    XGrabButton(dpy, AnyButton, Mod1Mask|Mod4Mask, root, True, ButtonPressMask, GrabModeAsync, GrabModeAsync, None, None);
    XEvent ev;
	while (running && !XNextEvent(dpy,&ev))
		if (handler[ev.type])
			handler[ev.type](&ev);
	return 0;
}


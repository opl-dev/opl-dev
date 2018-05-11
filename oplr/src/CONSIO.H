// CONSIO.H
//
// Copyright (c) 1997-1999 Symbian Ltd.  All rights reserved.
//

struct P_POINT
    {
    TInt16 x; /* Horizontal coordinate */
    TInt16 y; /* Vertical coordinate */
    };

struct P_RECT
    {
    P_POINT tl; /* Top left point */
    P_POINT br; /* Bottom right point */
    };

struct P_RECTP
    {
    P_RECT r; /* Rectangle */
    P_POINT p; /* Point */
    };

struct P_CON_KBREC
    {
    TUint16 keycode; /* Code for the key pressed */
    TUint8 modifiers; /* A bit set for each modifier key held down */
    TUint8 count;
    };

struct CONSOLE_INFO
	{
	TUint16 window_handle;
	TUint16 font_handle;
	TUint16 line_height;
	TUint16 char_width;
	};

#define P_MAXEDITSTR 255

struct P_CEDIT  /* console driver line editor */
    {
    TUint8 cursorpos;    /* initial cursor position */
    TUint8 trap;         /* trap input errors or not */
    TUint8 string[P_MAXEDITSTR]; /* buffer to edit */
    };

#define FEDIT     11     /* invoke consol edit */

#define P_SCR_ATTRB 0
#define P_SCR_SCROLL 1
#define P_SCR_CLR 2
#define P_SCR_POSA 3
#define P_SCR_POSR 4
#define P_SCR_WSET 5
#define P_SCR_SLOCK 6
#define P_SCR_WLOCK 7
#define P_SCR_NEL 8
#define P_SCR_CURSOR 9
#define P_SCR_LAST_LINE_WRAP 16

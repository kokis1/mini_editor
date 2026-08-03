#ifndef SETUP
#define SETUP

#include <curses.h>

void setup_screen() {
   initscr();              /* starts curses mode */

   noecho();               /* stops echo of characters on the screen */
   raw();                  /* enables raw terminal mode */
   keypad(stdscr, TRUE);   /* enables control characters and function keys */
}

void exit_program() {
   endwin();               /* end curses mode */
}


#endif
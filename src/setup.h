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
   echo();                 /* gets the echo back */
   noraw();                /* disables raw mode */
   keypad(stdscr, FALSE);  /* disables control characters and function keys */
   endwin();               /* end curses mode */
}


#endif
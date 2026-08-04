#ifndef SETUP
#define SETUP


/* GLOBAL HEADERS */
#include <curses.h>

/* LOCAL HEADERS  */
#include "data.h"

void fill_screen_buf() {
   /* fills the screen buffer with '~' in every row   */

   /* sets the cursor position to top-left            */
   int x, y;
   getyx(stdscr, y, x);

   /* sets the colour to purple */
   init_pair(1, COLOR_MAGENTA, -1);    /* the -1 means to use default colour*/
   attron(COLOR_PAIR(1));

   /* prints '~' until the bottom of the screen       */
   for (int y = 1; y < LINES; y++) {
      mvaddch(y, 0, '~');
   }

   /* turns off the colour                            */
   attroff(COLOR_PAIR(1));

   /* resets the cursor back to the top-left          */
   move(0, 0);
   refresh();

}

std_return setup_colour() {
   /* turns on colour for the terminal */
   if (!has_colors()) return QUIT;

   start_color();          /* initialises the colour of the terminal    */
   use_default_colors();   /* makes -1 the default colour               */

   return CONTINUE;
}

void setup_screen() {

   initscr();              /* starts curses mode                              */

   noecho();               /* stops echo of characters on the screen          */
   raw();                  /* enables raw terminal mode */
   keypad(stdscr, TRUE);   /* enables control characters and function keys    */
}

void exit_program() {
   echo();                 /* gets the echo back                              */
   noraw();                /* disables raw mode                               */
   keypad(stdscr, FALSE);  /* disables control characters and function keys   */
   endwin();               /* end curses mode                                 */
}


#endif
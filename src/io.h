#ifndef IO
#define IO

/* GLOBAL HEADERS */
#include <curses.h>

/* LOCAL HEADERS  */
#include "data.h"

void move_key(int c) {
   /* moves the cursor if a key is pressed               */

   /* gets the current position of the cursor            */
   int y, x;
   getyx(stdscr, y, x);


   /* mves the cursor, ensuring it stays on the screen   */
   switch (c) {
      case KEY_LEFT:
            if (x > 0) move(y, x - 1);
            break;
      case KEY_RIGHT:
            if (x < COLS) move(y, x + 1);
            break;
      case KEY_DOWN:
            if (y < LINES) move(y + 1, x);
            break;
      case KEY_UP:
            if (y > 0) move(y - 1, x);
            break;
   }
}

std_return take_input() {
   /* gets the input from the user and does things accordingly       */
   int c = getch();

   switch (c) {
      case KEY_LEFT:
      case KEY_RIGHT:
      case KEY_UP:
      case KEY_DOWN:
         move_key(c);
         break;
      case 'q':
      case 23:                /* decimal ascii value for ctrl+W: 23  */
         return QUIT;
      default:
         addch(c);
         break;
   }
   return CONTINUE;
}


#endif
#include <curses.h>
#include "setup.h"
#include "data.h"

int main(int argc, char** argv) {
   setup_screen();

   while(1) {

      char c = getch();       /* Gets user input*/

      if (c == 'q') break;    /* if the 'q' is pressed, exit */
      printw("%c", c);        /* otherwise, print to the screen buffer*/
      refresh();              /* write to the screen*/
   }

   exit_program();
   return 0;
}
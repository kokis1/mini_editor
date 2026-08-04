#include <curses.h>
#include "setup.h"
#include "data.h"
#include "io.h"

int main(int argc, char** argv) {

   setup_screen();

   if (setup_colour() != CONTINUE) {
      printf("EXITING: TERMINAL DOESN'T HAVE COLOUR");
      exit_program();
      return 0;
   }
   
   if (argc == 1){
      fill_screen_buf();         /* populates the screen with default values  */
   }

   while(1) {
      if (take_input() != CONTINUE) break;
      refresh();
   }

   exit_program();
   return 0;
}
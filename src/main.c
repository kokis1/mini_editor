#include <curses.h>

int main() {
   
   initscr(); // initialise the library
   noecho(); // do not echo the user's input
   cbreak(); // do not buffer user input, retain ctrl+C and ctrl+Z actions
   keypad(stdscr, TRUE); // enable extended characters (e.g. F keys and from the keypad)

   if (has_colors() == FALSE) {
      endwin();
      printf("Your terminal doesn't support colours");
      return 1;
   }

   start_color();

   init_pair(1, COLOR_WHITE, COLOR_BLUE); // foreground and background colours

   attron(COLOR_PAIR(1));

   int y, x;

   getmaxyx(stdscr, y, x);

   y = y * 0.5;
   x = (x * 0.5) - 6;
   mvwprintw(stdscr, y, x, "Hello, world!");
   mvwprintw(stdscr, 2, 2, "Hellow, world!");
   refresh();

   attroff(COLOR_PAIR(1));

   getch(); // wait for keypress before exiting

   endwin(); // gracefully close the window and let ncurses clean up
   return 0;
}
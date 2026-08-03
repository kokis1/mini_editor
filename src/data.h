#ifndef DATA
#define DATA

/* this contains all the declarations and initialisations of global data */

struct Screen {
   int cursor_x;
   int cursor_y;
   int screen_width;
   int screen_height;
   char *screen_buf;
};

struct Screen screen;


#endif
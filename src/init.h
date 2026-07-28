/* GLOBAL HEADERS */

/* LOCAL HEADERS */
#include "data.h"
#include "editorops.h"

#ifndef INIT
#define INIT

void init_editor() {
   E.cx = 0;
   E.cy = 0;
   E.num_rows = 0;
   E.row = NULL;
   E.rowoff = 0;
   E.coloff = 0;
   E.rx = 0;
   E.filename = NULL;
   E.status_msg[0] = '\0';
   E.syntax = NULL;
   E.status_msg_time = 0;
   E.dirty = 0;
   if(get_window_size(&E.screen_rows, &E.screen_cols) == -1) die("get_window_size");
   E.screen_rows -= 2;
}

#endif
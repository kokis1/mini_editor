#ifndef EDITOROPS
#define EDITOROPS

/* GLOBAL HEADERS */

/* LOCAL HEADERS*/
#include "data.h"
#include "terminal.h"
#include "rowops.h"
#include "syntaxhl.h"

void editor_insert_char(int c) {
   if (E.cy == E.num_rows) {
      editor_insert_row(E.num_rows, "", 0);
   }
   editor_row_insert_char(&E.row[E.cy], E.cx, c);
   E.cx++;
}

void editor_insert_newline() {
   if (E.cx == 0) {
      editor_insert_row(E.cy, "", 0);
   } else {
      erow *row = &E.row[E.cy];
      editor_insert_row(E.cy + 1, &row->chars[E.cx], row->size - E.cx);
      row = &E.row[E.cy];
      row->size = E.cx;
      row->chars[row->size] = '\0';
      editor_update_row(row);
   }
   E.cy++;
   E.cx = 0;

}

void editor_del_char() {
   if (E.cy == E.num_rows) return;
   if (E.cx == 0 && E.cy == 0) return;

   erow *row = &E.row[E.cy];
   if (E.cx > 0) {
      editor_row_del_char(row, E.cx - 1);
      E.cx--;
   } else {
      E.cx = E.row[E.cy - 1].size;
      editor_append_str(&E.row[E.cy - 1], row->chars, row->size);
      editor_del_row(E.cy);
      E.cy--;
   }
}

#endif
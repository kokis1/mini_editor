/* GLOBAL HEADERS */
#include <ctype.h>
#include <stdio.h>



/* LOCAL HEADERS*/
#include "rowops.h"
#include "defines.h"
#include "editorops.h"
#include "fileio.h"


#ifndef IO
#define IO

char *editor_prompt(char *prompt, void (*callback)(char *, int)) {
	size_t bufsize = 128;
	char *buf = (char*)malloc(bufsize);

	size_t buflen = 0;
	buf[0] = '\0';

	while (1) {
		editor_status_msg(prompt, buf);
		editor_refresh_screen();

		int c = editor_read_key();
		if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
				if (buflen != 0) buf[--buflen] = '\0';
		} else if (c == '\x1b') {
			editor_status_msg("");
			if (callback) callback(buf, c);
			free(buf);
			return NULL;
		} else if (c == '\r') {
			if (buflen != 0) {
			editor_status_msg("");
			if (callback) callback(buf, c);
			return buf;
		}
		} else if (!iscntrl(c) && c < 128) {
			if (buflen == bufsize - 1) {
			bufsize *= 2;
			buf = (char *)realloc(buf, bufsize);
		}		 
		buf[buflen++] = c;
		buf[buflen] = '\0';
	}
	if (callback) callback(buf, c);
	}
}
void editor_move_cursor(int key) {
   erow *row = (E.cy >= E.num_rows) ? NULL : &E.row[E.cy];


   switch (key) {
      case ARROW_LEFT:
         if(E.cx != 0) {
         E.cx--;
         } else if (E.cy > 0) {
            E.cy--;
            E.cx = E.row[E.cy].size;
         }
         break;
      case ARROW_RIGHT:
         if(row && E.cx < row->size) {
         E.cx++;
         } else if (row && E.cx == row->size) {
            E.cy++;
            E.cx = 0;
         }
         break;
      case ARROW_UP:
         if (E.cy != 0) {
         E.cy--;
         }
         break;
      case ARROW_DOWN:
         if (E.cy < E.num_rows) {   
            E.cy++;
         }
         break;
   }


   row = (E.cy >= E.num_rows) ? NULL : &E.row[E.cy];
   int row_len = row ? row->size : 0;
   if (E.cx > row_len) {
      E.cx = row_len;
   }
}

void editor_process_key_press() {
   static int quit_times = QUIT_TIMES;

   int c = editor_read_key();
   switch (c) {
      case CTRL_KEY('w'):
         if (E.dirty && quit_times > 0) {
            editor_status_msg("WARNING!! File has unsaved changes."
            "Press CTRL-W %d more times to quit.", quit_times);
            quit_times--;
            return;
         }
         write(STDOUT_FILENO, "\x1b[2J", 4);
         write(STDOUT_FILENO, "\x1b[H", 3);
         exit(0);
         break;

      case '\r':
         editor_insert_newline();
         break;

      case HOME_KEY:
         E.cx = 0;
         break;
      case BACKSPACE:
      case CTRL_KEY('h'):
      case DEL_KEY:
         if (c == DEL_KEY) editor_move_cursor(ARROW_RIGHT);
         editor_del_char();
         break;
      case END_KEY:
        if (E.cy < E.num_rows) E.cx = E.row[E.cy].size;
         break;

      case PAGE_DOWN:
      case PAGE_UP:
         {
            if (c == PAGE_UP) {
               E.cy = E.rowoff;
            } else if (c == PAGE_DOWN) {
               E.cy = E.rowoff + E.screen_rows - 1;
               if (E.cy > E.num_rows) E.cy = E.num_rows;
            }
            int times = E.screen_rows;
            while (times--) editor_move_cursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
         }
         break;
      
      case CTRL_KEY('s'):
         editor_save();
         break;

      case CTRL_KEY('f'):
	  editor_find();
	  break;

      case CTRL_KEY('l'):
      case '\x1b':
         /* TODO */
         break;
      case ARROW_DOWN:
      case ARROW_UP:
      case ARROW_LEFT:
      case ARROW_RIGHT:
         editor_move_cursor(c);
         break;
      default:
         editor_insert_char(c);
         break;
   }

   quit_times = QUIT_TIMES;
}

/* OUTPUT */

void editor_scroll() {
   E.rx = 0;

   if (E.cy < E.num_rows) {
      E.rx = editor_row_cx_to_rx(&E.row[E.cy], E.cx);
   }

   if (E.cy < E.rowoff) {
      E.rowoff = E.cy;
   }
   if (E.cy >= E.rowoff + E.screen_rows) {
      E.rowoff = E.cy - E.screen_rows + 1;
   }
   if (E.rx < E.coloff) {
      E.coloff = E.rx;
   }
   if (E.rx >= E.coloff + E.screen_cols) {
      E.coloff = E.rx - E.screen_cols + 1;
   }
}

void editor_draw_rows(struct abuf *ab) {
	int y;
	for (y = 0; y < E.screen_rows; y++) {
	int filerow = y + E.rowoff;
 	if (filerow >= E.num_rows) {
		if (E.num_rows == 0 && y == E.screen_rows / 3) {
            		char welcome[80];
            		int welcomelen = snprintf(welcome, sizeof(welcome),
                                       "Kilo Editor -- version %s", KILO_VERSION);
            		if (welcomelen > E.screen_cols) welcomelen = E.screen_cols;
            		int padding = (E.screen_cols - welcomelen) / 2;

            		if (padding) {
               			ab_append(ab, "~", 1);
               			padding--;
            		}
            		while (padding--) ab_append(ab, " ", 1);

		        ab_append(ab, welcome, welcomelen);
            	} else {
               		ab_append(ab, "~", 1);
            	  }
      	} else {
        	int len = E.row[filerow].r_size - E.coloff;
        	if(len < 0) len = 0;
        	if (len > E.screen_cols) len = E.screen_cols;
		char *c = &E.row[filerow].render[E.coloff];
		unsigned char *hl = &E.row[filerow].hl[E.coloff];
		int current_colour = -1;
		int j; 
		for (j = 0; j < len; j++) {
			if (iscntrl(c[j])) {
				char sym = (c[j] <= 26) ? '@' + c[j] : '?';
				ab_append(ab, "\x1b[7m", 4);
				ab_append(ab, &sym, 1);
				ab_append(ab, "\x1b[m", 3);
				if (current_colour != -1) {
					char buf[16];
					int c_len = snprintf(buf, sizeof(buf), "\x1b[%dm", current_colour);
					ab_append(ab, buf, c_len);
				}
			} else if (hl[j] == HL_NORMAL) {
				if (current_colour != -1) {
					ab_append(ab, "\x1b[39m", 5);
					current_colour = -1;
				}	
				ab_append(ab, &c[j], 1);
			} else {
				int colour = editor_syntax_to_colour(hl[j]);
				if (colour != current_colour) {
					current_colour = colour;
					char buf[16];
					int c_len = snprintf(buf, sizeof(buf), "\x1b[%dm", colour);
					ab_append(ab, buf, c_len);
				}
				ab_append(ab, &c[j], 1);
			}
		}
		ab_append(ab, "\x1b[39m", 5);     
	}

	ab_append(ab, "\x1b[K", 3);
	ab_append(ab, "\r\n", 2);
	}
}

void editor_draw_msg_bar(struct abuf *ab) {
   ab_append(ab, "\x1b[K", 3);
   int msg_len = strlen(E.status_msg);
   if (msg_len > E.screen_cols) msg_len = E.screen_cols;
   if (msg_len && time(NULL) - E.status_msg_time < 5) ab_append(ab, E.status_msg, msg_len);
}

void editor_draw_status_bar(struct abuf *ab) {
	ab_append(ab, "\x1b[7m", 4);
	char status[80], r_status[80];
	int len = snprintf(status, sizeof(status), "%.20s - %d lines %s",
	E.filename ? E.filename : "[No Name]", E.num_rows,
	E.dirty ? "(modified)" : "");
	int r_len = snprintf(r_status, sizeof(r_status), "%s | %d/%d", 
		E.syntax ? E.syntax->file_type : "No ft", E.cy + 1, E.num_rows);
	if (len > E.screen_cols) len = E.screen_cols;
	ab_append(ab, status, len);
	while (len < E.screen_cols) {
		if (E.screen_cols - len == r_len) {
			ab_append(ab, r_status, r_len);
         		break;
      		} else {
         		ab_append(ab, " ", 1);
         	len++;
      		}
   	}
   	ab_append(ab, "\x1b[m", 3);
   	ab_append(ab, "\r\n", 2);
}

void editor_refresh_screen() {
   editor_scroll();

   struct abuf ab = ABUF_INIT;

   ab_append(&ab, "\x1b[?25l", 6);
   ab_append(&ab, "\x1b[H", 3);
   
   editor_draw_rows(&ab);
   editor_draw_status_bar(&ab);
   editor_draw_msg_bar(&ab);

   char buf[32];
   snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowoff) + 1, (E.rx - E.coloff) + 1);
   ab_append(&ab, buf, strlen(buf));
   

   ab_append(&ab, "\x1b[?25h", 6);

   write(STDOUT_FILENO, ab.b, ab.len);
   ab_free(&ab);
}

void editor_status_msg(const char *fmt, ...) {
   va_list ap;
   va_start(ap, fmt);
   vsnprintf(E.status_msg, sizeof(E.status_msg), fmt, ap);
   va_end(ap);
   E.status_msg_time = time(NULL);
}

#endif
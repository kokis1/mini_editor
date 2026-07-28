
/* GLOBAL HEADERS */
#include <time.h>
#include <termios.h>

/* LOCAL HEADERS */


#ifndef DATA
#define DATA
struct editorSyntax {
	char *file_type;
	char **file_match;
	char **keywords;
	char *single_line_comment_start;
	char *multi_line_comment_start;
	char *multi_line_comment_end;
	int flags;
};


typedef struct erow {
	int idx;
	int size;
 	int r_size;
	char *chars;
	char *render;
	unsigned char *hl;
	int hl_open_comment;
} erow;

struct EditorConfig {
	int cx, cy;
	int rx;
	int rowoff;
	int coloff;
	int screen_rows;
	int screen_cols;
	int num_rows;
	erow *row;
	int dirty;
	char *filename;
	char status_msg[80];
	time_t status_msg_time;
	struct editorSyntax *syntax;
	struct termios orig_termios;
};

struct EditorConfig E;

#endif
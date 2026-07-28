#ifndef FILEIO
#define FILEIO

/* GLOBAL HEADERS */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


/* LOCAL HEADERS */
#include "defines.h"
#include "data.h"
#include "terminal.h"
#include "rowops.h"
#include "editorops.h"
#include "io.h"
#include "syntaxhl.h"

char *editor_row_tostr(int *buflen) {
   int tot_len = 0;
   int j;
   for (j = 0; j < E.num_rows; j++)
      tot_len += E.row[j].size + 1;
   
   *buflen = tot_len;

   char *buf = (char *)malloc(tot_len);
   char *p = buf;

   for (j = 0; j < E.num_rows; j++) {
      memcpy(p, E.row[j].chars, E.row[j].size);
      p += E.row[j].size;
      *p = '\n';
      p++;
   }

   return buf;
}

void editor_open(char *filename) {
	free(E.filename);
	E.filename = strdup(filename);

	editor_select_syntax_highlight();

	FILE *fp = fopen(filename, "r");
	if (!fp) die("fopen");

	char *line = NULL;
	size_t linecap = 0;
	ssize_t linelen;

	while ((linelen = getline(&line, &linecap, fp)) != -1){
		while (linelen > 0 && (line[linelen - 1] == '\n'
                              || line[linelen - 1] == '\r'))
			linelen--;
		editor_insert_row(E.num_rows, line, linelen);
	}   

	free(line);
	fclose(fp);
	E.dirty = 0;
}

void editor_save() {
	if (E.filename == NULL) {
		E.filename = editor_prompt("Save as: %s (ESC to cancel)", NULL);
		if (E.filename == NULL) {
		editor_status_msg("Save aborted");
		return;
	}
	editor_select_syntax_highlight();
}
   
   int len;
   char *buf = editor_row_tostr(&len);

   int fd = open(E.filename, O_RDWR | O_CREAT, 0644);
   if (fd != -1) {
      if (ftruncate(fd, len) != -1) {
         if (write(fd, buf, len) == len) {
            close(fd);
            free(buf);
            E.dirty = 0;
            editor_status_msg("%d bytes written to the disk", len);
            return;
         }
      }
      close(fd);
   }
   free(buf);
   editor_status_msg("Can't save! I/O error %s", strerror(errno));
}


#endif
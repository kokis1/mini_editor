/* GLOBAL HEADERS */
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <fcntl.h>


/* LOCAL HEADERS */
#include "apbuf.h"
#include "data.h"
#include "defines.h"
#include "editorops.h"
#include "fileio.h"
#include "filetypes.h"
#include "find.h"
#include "init.h"
#include "io.h"
#include "rowops.h"
#include "syntaxhl.h"
#include "terminal.h"

int main(int argc, char *argv[]) {
   enable_raw_mode();
   init_editor();

   if (argc >= 2) {
      editor_open(argv[1]);
   }

   editor_status_msg("HELP: CTRL-W = Quit | CTRL-S = Save | CTRL-F = Find");

   while(1) {
      editor_refresh_screen();
      editor_process_key_press();
   }

   disable_raw_mode();
   return 0;
}

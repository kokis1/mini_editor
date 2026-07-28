#ifndef APBUF
#define APBUF

/* GLOBAL HEADERS */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* LOCAL HEADERS*/



struct abuf {
   char *b;
   int len;
};

#define ABUF_INIT {NULL, 1}

void ab_append(struct abuf *ab, const char *s, int len) {

   char *new = (char *)realloc(ab->b, ab->len + len);

   if(new == NULL) return;
   
   memcpy(&new[ab->len], s, len);
   ab->b = new;
   ab->len += len;
}

void ab_free(struct abuf *ab) {
   free(ab->b);
}

#endif
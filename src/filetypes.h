/* GLOBAL HEADERS*/
#include <stdlib.h>

/* LOCAL HEADERS */
#include "defines.h"
#include "data.h"


#ifndef FILETYPES
#define FILETYPES
char *C_HL_extensions[] = {".c", ".cpp", ".h", NULL};
char *C_HL_KEYWORDS[] = {
	"switch", "if", "while", "for", "break", "continue", "return", "else",
	"struct", "union", "typedef", "static", "enum", "class", "case",

	"int|", "long|", "double|", "float|", "char|", "unsigned|", "signed|",
	"void|", NULL
};


struct editorSyntax HLDB[] = {
	{
		"c",
		C_HL_extensions,
		C_HL_KEYWORDS,
		"//", "/*", "*/",
		HL_HIGHLIGHT_STRINGS | HL_HIGHLIGHT_NUMBERS,
	},
};

#define HLDB_ENTRIES (sizeof(HLDB) / sizeof(HLDB[0]))

#endif
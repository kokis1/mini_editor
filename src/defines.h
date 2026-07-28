
#ifndef DEFINES
#define DEFINES

#define CTRL_KEY(k) ((k) & 0x1f)

#define KILO_VERSION "0.0.1"
#define TAB_STOP 8
#define QUIT_TIMES 3

#define HL_HIGHLIGHT_NUMBERS (1<<0)
#define HL_HIGHLIGHT_STRINGS (1<<1)

enum editor_key {
   BACKSPACE = 127,
   ARROW_LEFT = 1000,
   ARROW_RIGHT,
   ARROW_UP,
   ARROW_DOWN,
   HOME_KEY,
   END_KEY,
   DEL_KEY,
   PAGE_UP,
   PAGE_DOWN
};

enum editor_highlight {
	HL_NORMAL = 0,
	HL_COMMENT,
	HL_MCOMMENT,
	HL_KEYWORD1,
	HL_KEYWORD2,
	HL_STRING,
	HL_NUMBER,
	HL_MATCH
};

#endif
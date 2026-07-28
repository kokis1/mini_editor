/* GLOBAL HEADERS */



/* LOCAL HEADERS*/
#include "defines.h"
#include "data.h"
#include "filetypes.h"


#ifndef SYNTAXHL
#define SYNTAXHL

int is_separator(int c) {
	return isspace(c) || c == '\0' || strchr(".,()+-/*=~%<>[];", c) != NULL;
}


void editor_update_syntax(erow *row) {
	row->hl = realloc(row->hl, row->r_size);
	memset(row->hl, HL_NORMAL, row->r_size);

	if (E.syntax == NULL) return;

	char **keywords = E.syntax->keywords;

	char *s_comment_strt = E.syntax->single_line_comment_start;
	char *m_comment_strt = E.syntax->multi_line_comment_start;
	char *m_comment_end = E.syntax->multi_line_comment_end;

	int s_comment_strt_len = s_comment_strt ? strlen(s_comment_strt) : 0;
	int m_comment_strt_len = m_comment_strt ? strlen(m_comment_strt) : 0;
	int m_comment_end_len = m_comment_end ? strlen(m_comment_end) : 0;

	int prev_sep = 1;
	int in_string = 0;
	int in_comment = (row->idx > 0 && E.row[row->idx - 1].hl_open_comment);
	
	int i = 0;
	while (i < row->r_size) {
		char c = row->render[i];
		unsigned char prev_hl = (i > 0) ? row->hl[i - 1] : HL_NORMAL;

		if (s_comment_strt_len && !in_string && !in_comment) {
			if(!strncmp(&row->render[i], s_comment_strt, s_comment_strt_len)) {
				memset(&row->hl[i], HL_COMMENT, row->r_size - i);
				break;
			}
		}
		if (m_comment_strt_len && m_comment_end_len && !in_string) {
			if (in_comment) {
				row->hl[i] = HL_MCOMMENT;
				if (!strncmp(&row->render[i], m_comment_end, m_comment_end_len)) {
					memset(&row->hl[i], HL_MCOMMENT, m_comment_end_len);
					i += m_comment_end_len;
					in_comment = 0;
					prev_sep = 1;
					continue;
				} else {
					i++;
					continue;
				    }
			} else if (!strncmp(&row->render[i], m_comment_strt, m_comment_strt_len)) {
				memset(&row->hl[i], HL_MCOMMENT, m_comment_strt_len);
				i += m_comment_strt_len;
				in_comment = 1;
				continue;
			}
		}


		if (E.syntax->flags &HL_HIGHLIGHT_STRINGS) {
			if (in_string) {
				row->hl[i] = HL_STRING;
				if (c == '\\' && i + 1 < row->r_size) {
					row->hl[i + 1] = HL_STRING;
					i += 2;
					continue;
				}
				if (c == in_string) in_string = 0;
				i++;
				prev_sep = 1;
				continue;
			} else {
				if (c == '"' || c == '\'') {
					in_string = c;
					row->hl[i] = HL_STRING;
					i++;
					continue;
				}
			}				
		}

		if (E.syntax->flags & HL_HIGHLIGHT_NUMBERS) {
			if ((isdigit(c) && (prev_sep || prev_hl == HL_NUMBER)) ||
						 (c == '.' && prev_hl == HL_NUMBER)) {
				row->hl[i] = HL_NUMBER;
				i++;
				prev_sep = 0;
				continue;
			}
		}
		if (prev_sep) {
			int j;
			for (j = 0; j < keywords[j]; j++) {
				int k_len = strlen(keywords[j]);
				int kw2 = keywords[j][k_len - 1] == '|';
				if (kw2) k_len--;

				if (!strncmp(&row->render[i], keywords[j], k_len)
					 && is_separator(row->render[i + k_len])) {
					memset(&row->hl[i], kw2 ? HL_KEYWORD2 : HL_KEYWORD1, k_len);
					i += k_len;
					break;
				}
			}
			if (keywords[j] == NULL) {
				prev_sep = 0;
				continue;
			}
		}

		prev_sep = is_separator(c);
		i++;
	}
	int changed = (row->hl_open_comment != in_comment);
	row->hl_open_comment = in_comment;
	if (changed && row->idx + 1 < E.num_rows)
		 editor_update_syntax(&E.row[row->idx + 1]);
}

int editor_syntax_to_colour(int hl) {
	switch(hl) {
		case HL_NUMBER: return 31;
		case HL_MATCH: return 34;
		case HL_KEYWORD1: return 33;
		case HL_KEYWORD2: return 32;
		case HL_COMMENT:
		case HL_MCOMMENT: return 36;
		case HL_STRING: return 35;
		default: return 37;
	}
}

void editor_select_syntax_highlight() {
	E.syntax = NULL;
	if (E.filename == NULL) return;
	
	char *extension = strrchr(E.filename, '.');
	
	for (unsigned int j = 0; j < HLDB_ENTRIES; j++) {
		struct editorSyntax *s = &HLDB[j];
		unsigned int i = 0;
		while (s->file_match[i]) {
			int is_ext = (s->file_match[i][0] == '.');
			if ((is_ext && extension && !strcmp(extension, s->file_match[i])) ||
				(!is_ext && strstr(E.filename, s->file_match[i]))) {
				E.syntax = s;
				
				int file_row;
				for (file_row = 0; file_row < E.num_rows; file_row++) {
					editor_update_syntax(&E.row[file_row]);
				}
				return;
			}
		   i++;
		
		}
   }

}

#endif
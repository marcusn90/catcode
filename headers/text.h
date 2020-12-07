#ifndef TEXT_H
#define TEXT_H

#include "types.h"

#define TEXT_LINE_MAX_LENGTH 1024

typedef struct TextLine {
  struct TextLine *next;
  char buf[TEXT_LINE_MAX_LENGTH];
} TextLine;

ui32 text_lines_count(TextLine *lines);
TextLine *text_to_lines(char *text);
char *lines_to_text(TextLine *lines);

#endif
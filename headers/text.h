#ifndef TEXT_H
#define TEXT_H

#define TEXT_LINE_MAX_LENGTH 1024

typedef struct TextLine {
  struct TextLine *next;
  char buf[TEXT_LINE_MAX_LENGTH];
} TextLine;

#endif
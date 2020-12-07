#include "headers/text.h"
#include "headers/types.h"
#include <assert.h>
#include <string.h>

ui32 text_lines_count(TextLine *lines) {
  ui32 res = 0;
  TextLine *line = lines;
  while (line != NULL) {
    ++res;
    line = line->next;
  }
  return res;
}
TextLine *text_to_lines(char *text) {
  assert(text != NULL);
  TextLine *lines = calloc(1, sizeof(TextLine));
  lines->next = NULL;
  TextLine *last_line = lines;
  char str[TEXT_LINE_MAX_LENGTH];
  ui32 len = 0;
  char c;
  ui32 i = 0;
  while ((c = text[i++]) != '\0') {
    if (c == '\n') {
      str_copy(last_line->buf, str, len);
      len = 0;
      memset(str, 0, TEXT_LINE_MAX_LENGTH);
      TextLine *new_line = calloc(1, sizeof(TextLine));
      new_line->next = NULL;
      last_line->next = new_line;
      last_line = new_line;
    } else {
      str[len++] = (char)c;
    }
  }
  // check for last line without \n
  if (len) {
    str_copy(last_line->buf, str, len);
  }
  return lines;
}

char *lines_to_text(TextLine *lines) {
  assert(lines != NULL);
  ui32 lines_num = text_lines_count(lines);
  char *text = calloc(lines_num * TEXT_LINE_MAX_LENGTH, sizeof(char));
  TextLine *line = lines;

  ui32 line_len = 0;
  char ch_new_line = '\n';
  while (line != NULL) {
    assert(line->buf);
    strcat(text, line->buf);
    strcat(text, &ch_new_line);
    line = line->next;
  }

  return text;
}

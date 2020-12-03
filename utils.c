#include "headers/utils.h"
#include "headers/types.h"
#include <string.h>

void str_pad_left(const char *src, char *dst, ui16 width) {
  ui16 src_len = strlen(src);
  i16 diff = width - src_len;
  ui16 i;
  for (i = 0; i < width; ++i) {
    if (i < diff) {
      dst[i] = ' ';
    } else {
      dst[i] = src[i - diff];
    }
  }
}

uchar is_whitespace(char c) { return c == ' ' || c == '\t'; }

uchar is_letter(char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

uchar is_digit(char c) { return c >= '0' && c <= '9'; }

uchar is_separator(char c) {
  switch (c) {
  case '-':
  case '/':
  case '\\':
  case '\'':
  case '"':
  case '.':
  case ',':
  case '<':
  case '>':
  case '#':
  case '*':
  case '[':
  case ']':
  case '=':
  case '{':
  case '}':
  case '(':
  case ')':
    return 1;
  default:
    return 0;
  }
}

CharType get_char_type(char c) {
  if (is_letter(c)) {
    return LETTER;
  } else if (is_digit(c)) {
    return DIGIT;
  } else if (is_whitespace(c)) {
    return WHITESPACE;
  } else if (is_separator(c)) {
    return SEPARATOR;
  } else {
    return OTHER;
  }
}
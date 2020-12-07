#ifndef UTILS_H
#define UTILS_H

#include "types.h"
typedef enum CharType { LETTER, DIGIT, WHITESPACE, SEPARATOR, OTHER } CharType;
void str_pad_left(const char *src, char *dst, ui16 width);
uchar is_whitespace(char c);
uchar is_letter(char c);
uchar is_digit(char c);
uchar is_separator(char c);
CharType get_char_type(char c);
void str_copy(char *dst, const char *src, int size);

#endif

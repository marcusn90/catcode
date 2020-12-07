#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include "text.h"
#include "types.h"

#define CLIPBOARD_SIZE 4096
struct {
  char text[CLIPBOARD_SIZE];
} Clipboard;

void clipboard_init();
void clipboard_write_str(char *str);
void clipboard_clear();

#endif
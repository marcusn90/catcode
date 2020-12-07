#include "headers/clipboard.h"
#include "headers/app_state.h"
#include "headers/text.h"
#include <assert.h>
#include <string.h>

void clipboard_init() { clipboard_clear(); }

void clipboard_write_str(char *str) {
  assert(str != NULL);

  clipboard_clear();
  str_copy(Clipboard.text, str, CLIPBOARD_SIZE);
}

void clipboard_clear() { memset(Clipboard.text, 0, CLIPBOARD_SIZE); }

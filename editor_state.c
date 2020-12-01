#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "headers/editor_state.h"
#include "headers/utils.h"

i32 editor_init_from_file(EditorBuffer *eb, FILE *f) {
  i32 lines_num = 0;
  eb->lines_head = calloc(1, sizeof(TextLine));

  TextLine *last_line = eb->lines_head;
  char str[TEXT_LINE_MAX_LENGTH];
  ui32 len = 0;
  char c;
  while ((c = fgetc(f)) != EOF) {
    if (c == '\n') {
      lines_num++;
      strncpy(last_line->buf, str, len);
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
    strncpy(last_line->buf, str, len);
    TextLine *new_line = calloc(1, sizeof(TextLine));
    new_line->next = NULL;
    last_line->next = new_line;
    lines_num++;
  }
  eb->total_lines_num = lines_num > 0 ? lines_num : 1;
  editor_set_mode_normal(eb);
  return eb->total_lines_num;
}
void editor_init_empty(EditorBuffer *eb) {
  eb->lines_head = calloc(1, sizeof(TextLine));
  editor_clear_line(eb->lines_head);
  eb->total_lines_num = 1;
  editor_set_mode_normal(eb);
}

void editor_set_mode_normal(EditorBuffer *eb) { eb->mode = EDITOR_MODE_NORMAL; }

void editor_set_mode_insert(EditorBuffer *eb) { eb->mode = EDITOR_MODE_INSERT; }

char editor_scroll_n_lines(EditorBuffer *eb, i32 n) {
  if (n > 0) {
    if (eb->total_lines_num > eb->max_visible_lines &&
        eb->first_visible_line_idx <=
            eb->total_lines_num - eb->min_visible_lines_after_scroll) {
      eb->first_visible_line_idx += n;
      return 1;
    }
  } else if (n < 0) {
    if (eb->first_visible_line_idx > 1) {
      eb->first_visible_line_idx += n;
      return 1;
    }
  }

  return -1;
}

void editor_adjust_cursor_on_active_line(EditorBuffer *eb) {
  TextLine *line = editor_active_line(eb);
  i32 line_len = strlen(line->buf);
  if (eb->cursor_pos > line_len) {
    eb->cursor_pos = line_len;
  }
  if (eb->cursor_pos < 0) {
    eb->cursor_pos = 0;
  }
  printf("Active line index: %d\n", eb->active_line_idx);
}

// currently n is only used to determ dir(up/down)
// but we move by 1
// TODO: add support to move by n
char editor_adjust_active_line(EditorBuffer *eb, i32 n) {
  if (n > 0) {
    if (eb->active_line_idx < eb->total_lines_num) {
      ++(eb->active_line_idx);
      if (eb->active_line_idx >
          eb->first_visible_line_idx + eb->max_visible_lines - 1) {
        ++(eb->first_visible_line_idx);
      }
      editor_adjust_cursor_on_active_line(eb);
      return 1;
    }
  } else if (n < 0) {
    if (eb->active_line_idx > 1) {
      --(eb->active_line_idx);
      if (eb->active_line_idx < eb->first_visible_line_idx) {
        --(eb->first_visible_line_idx);
      }

      editor_adjust_cursor_on_active_line(eb);
      return 1;
    }
  }
  return -1;
}

char editor_move_cursor(EditorBuffer *eb, i32 n) {
  if (n == 0)
    return -1;
  TextLine *line = editor_active_line(eb);
  i32 line_len = strlen(line->buf);

  // end of last line - no move right
  if (n > 0 && line->next == NULL && eb->cursor_pos == line_len) {
    return -1;
  }
  eb->cursor_pos += n;
  if (eb->cursor_pos < 0) {
    if (eb->active_line_idx == 1) {
      eb->cursor_pos = 0; // stay on the start of the first line
    } else {
      TextLine *prev_line = editor_nth_line(eb, eb->active_line_idx - 2);
      eb->cursor_pos = (i32)strlen(prev_line->buf);
      editor_adjust_active_line(eb, -1);
    }
  } else if (eb->cursor_pos > line_len) {
    eb->cursor_pos = 0;
    editor_adjust_active_line(eb, 1);
  }
  editor_adjust_cursor_on_active_line(eb);
  return 1;
}

TextLine *editor_nth_line(EditorBuffer *eb, i32 n) {
  i32 c = 0;
  TextLine *line = eb->lines_head;
  while (c < n) {
    c++;
    line = line->next;
  }
  return line;
}

TextLine *editor_active_line(EditorBuffer *eb) {
  return editor_nth_line(eb, eb->active_line_idx - 1);
}

void editor_line_chunk_before_cursor(EditorBuffer *eb, char *dst) {
  if (eb->cursor_pos == 0)
    return;

  TextLine *line = editor_active_line(eb);
  i32 len = strlen(line->buf);
  assert(eb->cursor_pos <= len);

  strncpy(dst, line->buf, eb->cursor_pos);
}

void editor_line_chunk_after_cursor(EditorBuffer *eb, char *dst) {
  TextLine *line = editor_active_line(eb);
  i32 len = strlen(line->buf);
  assert(eb->cursor_pos < len);

  strncpy(dst, line->buf + eb->cursor_pos, len - eb->cursor_pos);
}

void editor_clear_line(TextLine *line) {
  assert(line);
  assert(line->buf);
  memset(line->buf, 0, TEXT_LINE_MAX_LENGTH);
}

void editor_remove_char_before_cursor(EditorBuffer *eb) {
  if (eb->cursor_pos == 0 && eb->active_line_idx == 1)
    return; // [edge case] start of first line

  TextLine *line = editor_active_line(eb);
  i32 line_len = strlen(line->buf);

  // concat with the line above
  if (eb->cursor_pos == 0) {
    TextLine *prev_line = editor_nth_line(eb, eb->active_line_idx - 2);
    i32 new_cursor_pos = strlen(prev_line->buf);
    strcat(prev_line->buf, line->buf);
    prev_line->next = line->next; // remove current line
    --(eb->total_lines_num);
    editor_adjust_active_line(eb, -1);
    eb->cursor_pos = new_cursor_pos;
    free(line);
    line = NULL;
  } else {
    char after[TEXT_LINE_MAX_LENGTH] = {0};
    char before[TEXT_LINE_MAX_LENGTH] = {0};
    if (eb->cursor_pos < line_len) {
      editor_line_chunk_after_cursor(eb, after);
    }
    --(eb->cursor_pos);
    editor_line_chunk_before_cursor(eb, before);
    TextLine *line = editor_active_line(eb);
    editor_clear_line(line);
    strncpy(line->buf, before, TEXT_LINE_MAX_LENGTH);
    strcat(line->buf, after);
  }
}

void editor_remove_char_at_cursor(EditorBuffer *eb) {
  TextLine *line = editor_active_line(eb);
  i32 line_len = strlen(line->buf);

  printf("Attempt to del: '%s'\n", line->buf);
  printf("Len: %d\n", line_len);

  // delete line and stay on next
  if (line_len == 0) {
    if (line->next == NULL) {
      return; // [edge case] last empty line
    }
    if (line == eb->lines_head) {
      eb->lines_head = line->next;
    } else {
      TextLine *prev = editor_nth_line(eb, eb->active_line_idx - 2);
      prev->next = line->next;
    }
    --(eb->total_lines_num);
    free(line);
    line = NULL;
  } else if (eb->cursor_pos == line_len) {
    if (line->next != NULL) {
      TextLine *next_line = line->next;
      strcat(line->buf, next_line->buf);
      line->next = next_line->next;
      free(next_line);
      next_line = NULL;
      --(eb->total_lines_num);
    }
  } else {
    char before[TEXT_LINE_MAX_LENGTH] = {0};
    char after[TEXT_LINE_MAX_LENGTH] = {0};
    editor_line_chunk_before_cursor(eb, before);
    editor_line_chunk_after_cursor(eb, after);
    printf("Before: '%s'\n", before);
    printf("After: '%s'\n", after);
    printf("After to copy: '%s'\n", (char *)(after + 1));
    printf("len before: %d\n", line_len);
    editor_clear_line(line);
    strncpy(line->buf, before, TEXT_LINE_MAX_LENGTH);
    if ((i32)strlen(after) > 1) {
      strcat(line->buf, (char *)(after + 1));
    }
    printf("len after: %d\n", (i32)strlen(line->buf));
  }
}

TextLine *editor_add_new_line_at_cursor(EditorBuffer *eb) {
  TextLine *line = editor_active_line(eb);
  assert(line);
  i32 line_len = strlen(line->buf);
  TextLine *new_line = calloc(1, sizeof(TextLine));
  if (eb->cursor_pos == 0) {
    if (line == eb->lines_head) {
      eb->lines_head = new_line;
      new_line->next = line;
    } else {
      TextLine *prev_line = editor_nth_line(eb, eb->active_line_idx - 2);
      prev_line->next = new_line;
      new_line->next = line;
    }
  } else {
    new_line->next = line->next;
    line->next = new_line;
    if (eb->cursor_pos < line_len) {
      char curr_line_text[TEXT_LINE_MAX_LENGTH] = {0};
      editor_line_chunk_before_cursor(eb, curr_line_text);
      editor_line_chunk_after_cursor(eb, new_line->buf);
      editor_clear_line(line);
      strncpy(line->buf, curr_line_text, TEXT_LINE_MAX_LENGTH);
    }
  }
  ++(eb->total_lines_num);
  editor_adjust_active_line(eb, 1);
  eb->cursor_pos = 0;
  return new_line;
}

void editor_insert_str_at_cursor(EditorBuffer *eb, char *str) {
  assert(eb != NULL);
  assert(str != NULL);

  i32 str_len = strlen(str);
  if (str_len == 0)
    return;

  // printf("Entered: '%s'\n", str);

  TextLine *line = editor_active_line(eb);
  i32 line_len = strlen(line->buf);

  if (line_len == 0) {
    assert(eb->cursor_pos == 0);
    strcat(line->buf, str);
    editor_move_cursor(eb, str_len);
  } else {
    char before[TEXT_LINE_MAX_LENGTH] = {0};
    char after[TEXT_LINE_MAX_LENGTH] = {0};
    editor_line_chunk_before_cursor(eb, before);
    if (eb->cursor_pos < line_len) {
      editor_line_chunk_after_cursor(eb, after);
    }
    editor_clear_line(line);
    strcat(line->buf, before);
    strcat(line->buf, str);
    strcat(line->buf, after);
    editor_move_cursor(eb, str_len);
  }
}

char editor_coords_to_cursor(EditorBuffer *eb, ui32 glyph_width,
                             ui32 line_height, ui32 x, ui32 y) {
  i32 new_cursor_pos = 0;
  i32 new_active_line_idx = 0;
  x -= eb->view_props.offset_x;
  y -= eb->view_props.offset_y;
  new_cursor_pos = x / glyph_width;
  new_active_line_idx = eb->first_visible_line_idx + y / line_height;
  if (new_active_line_idx > eb->total_lines_num) {
    new_active_line_idx = eb->total_lines_num;
  }
  if (new_cursor_pos != eb->cursor_pos ||
      new_active_line_idx != eb->active_line_idx - 1) {
    eb->cursor_pos = new_cursor_pos;
    eb->active_line_idx = new_active_line_idx;
    editor_adjust_cursor_on_active_line(eb);
    return 1;
  }

  return -1;
}

void editor_move_cursor_to_line_start(EditorBuffer *eb) { eb->cursor_pos = 0; }

void editor_move_cursor_to_line_end(EditorBuffer *eb) {
  TextLine *line = editor_active_line(eb);
  i32 line_len = strlen(line->buf);
  eb->cursor_pos = line_len;
  i32 x = 45;
  i32 dx = 333;
}

void editor_move_cursor_to_current_word_start(EditorBuffer *eb) {}

void editor_move_cursor_to_current_word_end(EditorBuffer *eb) {}

i32 editor_move_cursor_to_next_word_start(EditorBuffer *eb) {
  TextLine *line = editor_active_line(eb);
  assert(line);

  i32 line_len = strlen(line->buf);
  if (line_len == 0 || eb->cursor_pos == line_len) {
    eb->cursor_pos = 0;
    editor_adjust_active_line(eb, 1);
    return 1;
  } else {

    ui32 i = eb->cursor_pos + 1;

    CharType curr_char_type = OTHER, prev_char_type = OTHER;
    // skip chars of same type and whitespaces
    // TODO: we should probably also skip '_', '-', ... inside words
    while (i < line_len) {
      curr_char_type = get_char_type(line->buf[i]);
      prev_char_type = get_char_type(line->buf[i - 1]);
      if (curr_char_type == WHITESPACE || curr_char_type == prev_char_type) {
        ++i;
      } else {
        break;
      }
    }

    eb->cursor_pos = i;
    return 1;
  }
  return -1;
}

void editor_move_cursor_to_prev_word_start(EditorBuffer *eb) {}
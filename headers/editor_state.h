#ifndef EDITOR_CONTENT_H
#define EDITOR_CONTENT_H

#include "text.h"
#include "text_render.h"
#include "types.h"
#include <stdio.h>

typedef enum EditorMode { EDITOR_MODE_NORMAL, EDITOR_MODE_INSERT } EditorMode;

typedef struct EditorViewProps {
  ui32 width;
  ui32 height;
  i32 offset_x;
  i32 offset_y;
} EditorViewProps;

typedef struct EditorBuffer {
  char assoc_file[1024];
  EditorMode mode;
  EditorViewProps view_props;
  TextRenderProps tr_props;
  TextLine *lines_head; // head of TextLine list; first line
  ui32 total_lines_num;
  ui32 max_visible_lines;
  ui32 first_visible_line_idx;
  ui32 min_visible_lines_after_scroll; // min lines rendered after full scroll
  ui32 active_line_idx;
  i32 cursor_pos;
  ui32 main_text_offset_x;
  ui32 main_text_offset_y;
} EditorBuffer;

i32 editor_init_from_file(EditorBuffer *eb, char **file_path);
void editor_init_empty(EditorBuffer *eb);

void editor_set_mode_normal(EditorBuffer *eb);
void editor_set_mode_insert(EditorBuffer *eb);

char editor_scroll_n_lines(EditorBuffer *eb, i32 n);

char editor_adjust_active_line(EditorBuffer *eb, i32 n);
void editor_adjust_cursor_on_active_line(EditorBuffer *eb);

char editor_move_cursor(EditorBuffer *eb, i32 n);

TextLine *editor_nth_line(EditorBuffer *eb, i32 n);
TextLine *editor_active_line(EditorBuffer *eb);
TextLine *editor_add_new_line_at_cursor(EditorBuffer *eb);

void editor_clear_line(TextLine *line);

void editor_line_chunk_before_cursor(EditorBuffer *eb, char *dst);
void editor_line_chunk_after_cursor(EditorBuffer *eb, char *dst);

void editor_remove_char_before_cursor(EditorBuffer *eb);
void editor_remove_char_at_cursor(EditorBuffer *eb);
void editor_insert_str_at_cursor(EditorBuffer *eb, char *str);

char editor_coords_to_cursor(EditorBuffer *eb, ui32 glyph_width,
                             ui32 line_height, ui32 x, ui32 y);

void editor_move_cursor_to_line_start(EditorBuffer *eb);
void editor_move_cursor_to_line_end(EditorBuffer *eb);

void editor_move_cursor_to_current_word_start(EditorBuffer *eb);
void editor_move_cursor_to_current_word_end(EditorBuffer *eb);
i32 editor_move_cursor_to_next_word_start(EditorBuffer *eb);
void editor_move_cursor_to_prev_word_start(EditorBuffer *eb);

void editor_sync_with_active(EditorBuffer *active_eb, EditorBuffer *dst_eb);

#endif

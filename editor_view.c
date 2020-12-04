#include <SDL2/SDL.h>

#include "headers/editor_view.h"
#include "headers/utils.h"

ui32 editor_view_max_chars_per_line(EditorBuffer *eb) {
  return eb->view_props.width / eb->tr_props.glyph_width - 2;
}

void editor_view_draw(SDL_Renderer *renderer, EditorBuffer *eb,
                      uchar is_active) {
  SDL_Color main_text_color = {0x4a, 0xf6, 0x26, 255};
  SDL_Color line_numbers_color = {0, 255, 255, 255};
  SDL_Color active_line_hightlight_color = {0x11, 0x11, 0x11, 0x33};
  SDL_Color cursor_color = {0xff, 0x11, 0x11, 0x11};

  SDL_Rect line_highlight_rect = {0, 0, 0, 0};

  char total_lines_num_str[10];
  ui32 line_number_str_len = 0;
  i32 line_number_panel_width = 0;

  SDL_itoa(eb->total_lines_num, total_lines_num_str, 10);
  line_number_str_len = strlen(total_lines_num_str);
  eb->main_text_offset_x = 0; //
  line_number_panel_width = line_number_str_len * eb->tr_props.glyph_width;

  if (line_number_panel_width < MIN_LINE_NUMBER_PANEL_WIDTH_PX) {
    line_number_panel_width = MIN_LINE_NUMBER_PANEL_WIDTH_PX;
  }
  eb->main_text_offset_x = line_number_panel_width + 20;
  eb->main_text_offset_y = 0; // 20;

  if (is_active) {
    // draw active line highlight
    line_highlight_rect.w = eb->view_props.width;
    line_highlight_rect.h = eb->tr_props.line_height;
    line_highlight_rect.x = eb->view_props.offset_x;
    line_highlight_rect.y =
        eb->view_props.offset_y +
        eb->tr_props.line_height *
            (eb->active_line_idx - eb->first_visible_line_idx) +
        (eb->tr_props.line_height - eb->tr_props.font_size) / 2;

    SDL_SetRenderDrawColor(renderer, active_line_hightlight_color.r,
                           active_line_hightlight_color.g,
                           active_line_hightlight_color.b,
                           active_line_hightlight_color.a);
    SDL_RenderFillRect(renderer, &line_highlight_rect);
  }

  if (is_active) {
    // draw cursor
    i32 cursor_offset_x = eb->view_props.offset_x + eb->main_text_offset_x +
                          eb->cursor_pos * eb->tr_props.glyph_width;
    i32 cursor_offset_y =
        line_highlight_rect.y +
        (eb->tr_props.line_height - eb->tr_props.font_size) / 2;
    SDL_SetRenderDrawColor(renderer, cursor_color.r, cursor_color.g,
                           cursor_color.b, cursor_color.a);
    SDL_Rect cursor_rect = {cursor_offset_x, cursor_offset_y,
                            eb->tr_props.glyph_width, eb->tr_props.font_size};
    if (eb->mode == EDITOR_MODE_NORMAL) {
      SDL_RenderFillRect(renderer, &cursor_rect);
    } else if (eb->mode == EDITOR_MODE_INSERT) {
      SDL_RenderDrawRect(renderer, &cursor_rect);
    }
  }

  // draw main text
  i32 text_offset_x = eb->view_props.offset_x + eb->main_text_offset_x;
  i32 text_offset_y = eb->view_props.offset_y + eb->main_text_offset_y;
  text_renderer_draw_multiline(
      renderer, editor_view_max_chars_per_line(eb), &eb->tr_props,
      eb->lines_head, eb->first_visible_line_idx - 1, eb->max_visible_lines + 3,
      text_offset_x, text_offset_y, main_text_color);

  // draw line numbers
  char line_number_str[10] = {'0'};
  ui32 last_line_idx = eb->first_visible_line_idx + eb->max_visible_lines;
  if (last_line_idx > eb->total_lines_num) {
    last_line_idx = eb->total_lines_num;
  }

  char padded_line_num_str[line_number_str_len + 1];
  i32 line_num_offset_x = 0;
  i32 line_num_offset_y = 0;
  memset(padded_line_num_str, 0, line_number_str_len + 1);
  for (ui32 i = eb->first_visible_line_idx; i <= last_line_idx; i++) {
    SDL_itoa(i, line_number_str, 10);
    str_pad_left(line_number_str, padded_line_num_str, line_number_str_len);
    line_num_offset_x =
        eb->view_props.offset_x +
        0.5 * (eb->main_text_offset_x - line_number_panel_width);
    line_num_offset_y =
        eb->view_props.offset_y +
        eb->tr_props.line_height * (i - eb->first_visible_line_idx);

    text_renderer_draw_string(renderer, &eb->tr_props, padded_line_num_str,
                              line_num_offset_x, line_num_offset_y,
                              line_numbers_color);
  }
}
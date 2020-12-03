#include "headers/editor_state.h"
#include "headers/types.h"
#include <SDL2/SDL.h>

i32 editor_controls_handle_keydown(SDL_Event e, EditorBuffer *eb) {
  i32 should_render = 0;
  switch (e.key.keysym.scancode) {
  case SDL_SCANCODE_ESCAPE:
    editor_set_mode_normal(eb);
    SDL_StopTextInput();
    should_render = 1;
    break;

  case SDL_SCANCODE_I:
  case SDL_SCANCODE_A:
  case SDL_SCANCODE_C:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      SDL_StartTextInput();
      editor_set_mode_insert(eb);
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_UP:
    if (editor_adjust_active_line(eb, -1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_DOWN:
    if (editor_adjust_active_line(eb, 1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_RIGHT:
    if (SDL_GetModState() & KMOD_ALT) {
      editor_move_cursor_to_word_start_forward(eb);
    } else {
      editor_move_cursor(eb, 1);
    }
    should_render = 1;
    break;
  case SDL_SCANCODE_B:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      editor_move_cursor_to_word_start_backward(eb);
      should_render = 1;
    }
    break;
  case SDL_SCANCODE_E:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      editor_move_cursor_to_word_end_forward(eb);
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_W:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      editor_move_cursor_to_word_start_forward(eb);
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_LEFT:
    if (editor_move_cursor(eb, -1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_RETURN:
    editor_add_new_line_at_cursor(eb);
    should_render = 1;
    break;

  case SDL_SCANCODE_BACKSPACE:
    editor_remove_char_before_cursor(eb);
    should_render = 1;
    break;

  case SDL_SCANCODE_DELETE:
    editor_remove_char_at_cursor(eb);
    should_render = 1;
    break;

  case SDL_SCANCODE_HOME:
    editor_move_cursor_to_line_start(eb);
    should_render = 1;
    break;

  case SDL_SCANCODE_END:
    editor_move_cursor_to_line_end(eb);
    should_render = 1;
    break;

  case SDL_SCANCODE_TAB:
    editor_insert_str_at_cursor(eb, "    ");
    should_render = 1;
    break;
  } // switch

  return should_render;
}

i32 editor_controls_handle_mousewheel(SDL_Event e, EditorBuffer *eb) {
  i32 should_render = 0;
  if (e.wheel.y > 0) {
    if (editor_scroll_n_lines(eb, -1) != -1) {
      should_render = 1;
    }
  } else if (e.wheel.y < 0) {
    if (editor_scroll_n_lines(eb, 1) != -1) {
      should_render = 1;
    }
  }
  return should_render;
}

i32 editor_controls_handle_mouse_button_down(SDL_Event e, EditorBuffer *eb) {
  i32 should_render = 0;
  if (e.button.button == SDL_BUTTON_LEFT) {
    // TODO: editor should probably know about its text renderer and its
    // offsets make TR_props and main_text_offset_x/y part of
    // EditorBuffer struct
    if (editor_coords_to_cursor(eb, eb->tr_props.glyph_width,
                                eb->tr_props.line_height,
                                e.button.x - eb->main_text_offset_x,
                                e.button.y - eb->main_text_offset_y) != -1) {
      should_render = 1;
    }
  }
  return should_render;
}
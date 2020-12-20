#include "headers/app_input_handlers.h"
#include "headers/app_state.h"
#include "headers/editor_state.h"
#include "headers/types.h"
#include <SDL2/SDL.h>
#include <assert.h>

extern AppState GAS;

struct LastKeyDown {
  ui32 tick;
  char c
} last_key_down = {0, 0};

static update_last_keydown(char c) {
  last_key_down.c = c;
  last_key_down.tick = SDL_GetTicks();
}

static reset_last_keydown() { last_key_down.c = 0; }

i32 app_handle_keydown(SDL_Event e) {
  i32 should_render = 0;
  ui32 tick = SDL_GetTicks();

  EditorBuffer *eb = app_state_get_active_eb();
  assert(eb);

  switch (e.key.keysym.scancode) {
  case SDL_SCANCODE_BACKSLASH:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      app_state_toggle_split_buffers();
      should_render = 1;
    }
    break;
  case SDL_SCANCODE_1:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      app_state_set_active_buffer_idx(0);
      should_render = 1;
    }
    break;
  case SDL_SCANCODE_2:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      app_state_set_active_buffer_idx(1);
      should_render = 1;
    }
    break;
  case SDL_SCANCODE_ESCAPE:
    editor_set_mode_normal(eb);
    SDL_StopTextInput();
    should_render = 1;
    break;

  case SDL_SCANCODE_I:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_move_cursor_to_line_start(eb); // start editing from line start
      }
      SDL_StartTextInput();
      editor_set_mode_insert(eb);
      should_render = 1;
    }
    break;
  case SDL_SCANCODE_A:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_move_cursor_to_line_end(eb); // start editing from line end
      } else {
        editor_move_cursor(eb, 1);
      }
      SDL_StartTextInput();
      editor_set_mode_insert(eb);
      should_render = 1;
    }
    break;
  case SDL_SCANCODE_C:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_remove_line_chunk_after_cursor(eb);
        SDL_StartTextInput();
        editor_set_mode_insert(eb);
        should_render = 1;
      }
    }
    break;
  case SDL_SCANCODE_S:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_clear_line(editor_active_line(eb));
        editor_adjust_cursor_on_active_line(eb);
      } else {
        editor_remove_char_at_cursor(eb);
      }
      SDL_StartTextInput();
      editor_set_mode_insert(eb);
      should_render = 1;
    }
    break;
  case SDL_SCANCODE_O:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_insert_line_above_cursor(eb);
      } else {
        editor_insert_line_below_cursor(eb);
      }
      SDL_StartTextInput();
      editor_set_mode_insert(eb);
      should_render = 1;
    }
    break;
  case SDL_SCANCODE_X:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      editor_remove_char_at_cursor(eb);
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_K:
    if (eb->mode == EDITOR_MODE_NORMAL &&
        editor_adjust_active_line(eb, -1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_UP:
    if (editor_adjust_active_line(eb, -1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_J:
    if (eb->mode == EDITOR_MODE_NORMAL &&
        editor_adjust_active_line(eb, 1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_DOWN:
    if (editor_adjust_active_line(eb, 1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_L:
    if (eb->mode == EDITOR_MODE_NORMAL && editor_move_cursor(eb, 1)) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_RIGHT:
    if (editor_move_cursor(eb, 1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_H:
    if (eb->mode == EDITOR_MODE_NORMAL && editor_move_cursor(eb, -1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_LEFT:
    if (editor_move_cursor(eb, -1) != -1) {
      should_render = 1;
    }
    break;

  case SDL_SCANCODE_D:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_cut_line_after_cursor(eb);
      } else {
        if (last_key_down.c == 'd' && tick - last_key_down.tick < 200) {
          editor_cut_active_line(eb);
          reset_last_keydown();
        }
      }
      printf("Cut: '%s'\n", Clipboard.text);
    }

    update_last_keydown('d');
    should_render = 1;

    break;

  case SDL_SCANCODE_Y:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_copy_active_line(eb);
      } else {
        if (last_key_down.c == 'y' && tick - last_key_down.tick < 200) {
          editor_copy_active_line(eb);
          reset_last_keydown();
        }
      }
      printf("Copied: '%s'\n", Clipboard.text);
    }

    update_last_keydown('y');

    break;

  case SDL_SCANCODE_P:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_paste_line_before_active(eb);
      } else if (!(SDL_GetModState() & KMOD_CTRL)) {
        editor_paste_line_after_active(eb);
      }
    }
    if (SDL_GetModState() & KMOD_CTRL) {
      editor_paste_at_cursor(eb);
    }
    should_render = 1;
    break;

  case SDL_SCANCODE_B:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (SDL_GetModState() & KMOD_SHIFT) {
        editor_cut_line_before_cursor(eb);
      } else {
        editor_move_cursor_to_word_start_backward(eb);
      }
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
    editor_insert_str_at_cursor(eb, "  ");
    should_render = 1;
    break;

  case SDL_SCANCODE_Z:
    if (eb->mode == EDITOR_MODE_NORMAL) {
      if (last_key_down.c == 'z' && tick - last_key_down.tick < 200) {
        reset_last_keydown();
        should_render = editor_scroll_active_line_to_middle(eb) == -1 ? 0 : 1;
      }
      printf("Copied: '%s'\n", Clipboard.text);
    }

    update_last_keydown('z');

    break;

  } // switch

  return should_render;
}

i32 app_handle_mousewheel(SDL_Event e) {
  EditorBuffer *eb = app_state_get_active_eb();
  assert(eb);

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

i32 app_handle_mouse_button_down(SDL_Event e) {
  i32 should_render = 0;
  EditorBuffer *eb = app_state_get_active_eb();
  assert(eb);
  if (e.button.button == SDL_BUTTON_LEFT) {
    if (editor_coords_to_cursor(eb, eb->tr_props.glyph_width,
                                eb->tr_props.line_height,
                                e.button.x - eb->main_text_offset_x,
                                e.button.y - eb->main_text_offset_y) != -1) {
      should_render = 1;
    }
  }
  return should_render;
}
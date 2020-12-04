#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "headers/app_input_handlers.h"
#include "headers/editor_state.h"
#include "headers/editor_view.h"

#include "headers/text_render.h"
#include "headers/types.h"
#include "headers/utils.h"

#include "headers/app_state.h"

#define DEFAULT_FONT_PATH "fonts/Inconsolata-Regular.ttf"

extern AppState GAS;

int main(int argc, char **argv) {
  printf("Starting vzheditor...\n");

  SDL_Window *window = NULL;
  SDL_Renderer *renderer = NULL;

  SDL_Init(SDL_INIT_VIDEO);

  // Create an application window with the following settings:
  window = SDL_CreateWindow("Vzheditor", SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED, WIN_WIDTH, WIN_HEIGHT,
                            SDL_WINDOW_OPENGL);

  if (window == NULL) {
    printf("Could not create window: %s\n", SDL_GetError());
    return 1;
  }

  puts("Window open...");

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (renderer == NULL) {
    printf("Could not create renderer: %s\n", SDL_GetError());
    return 1;
  }

  app_state_init();

  // TODO: read fontpath from argv or select inside the editor
  TextRenderProps tr_props = {.glyph_width = 0,
                              .line_height = (ui16)16 * 1.4,
                              .font = NULL,
                              .font_size = 16};
  text_renderer_init(renderer, DEFAULT_FONT_PATH, &tr_props);

  // parse argv for file to open
  printf("ARGC: %d\n", argc);

  char **file_path = NULL;
  if (argc == 2) {
    file_path = &argv[1];
  }

  EditorBuffer eb_1 = {
      .tr_props = tr_props,
      .view_props = (EditorViewProps){WIN_WIDTH, WIN_HEIGHT, 0, 0},
      .max_visible_lines = (ui32)(WIN_HEIGHT / tr_props.line_height),
      // index of the top most line visible in editor
      .first_visible_line_idx = 1,
      // min lines rendered after full scroll
      .min_visible_lines_after_scroll = 5,
      .active_line_idx = 1,
      .cursor_pos = 0,
      .main_text_offset_x = 0,
      .main_text_offset_y = 0};

  GAS.open_editor_buffers[0] = &eb_1;
  GAS.active_eb_idx = 0;

  if (file_path != NULL) {
    editor_init_from_file(&eb_1, file_path);
  } else {
    editor_init_empty(&eb_1);
  }

  uchar should_render = 1; // first render
  uchar quit = 0;
  SDL_Event e;
  if (eb_1.mode == EDITOR_MODE_NORMAL) {
    SDL_StopTextInput();
  } else if (eb_1.mode == EDITOR_MODE_INSERT) {
    SDL_StartTextInput();
  }
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        puts("Quit");
        quit = 1;
        break;
      }

      switch (e.type) {
      case SDL_MOUSEBUTTONDOWN:
        should_render = app_handle_mouse_button_down(e);
        break;

      case SDL_MOUSEWHEEL:
        should_render = app_handle_mousewheel(e);
        break;

      case SDL_KEYDOWN:
        should_render = app_handle_keydown(e);
        break; // SDL_KEYDOWN

      case SDL_TEXTINPUT:
        editor_insert_str_at_cursor(app_state_get_active_eb(), e.text.text);
        should_render = 1;
        break;
      default:
        break;
      } // switch

      if (should_render) {
        if (GAS.open_editor_buffers[1] != NULL) {
          editor_sync_with_active(
              app_state_get_active_eb(),
              GAS.open_editor_buffers[1 - GAS.active_eb_idx]);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        for (i32 i = 0; i < MAX_OPEN_EDITOR_BUFFERS; ++i) {
          if (GAS.open_editor_buffers[i] != NULL) {
            editor_view_draw(renderer, GAS.open_editor_buffers[i],
                             (uchar)GAS.active_eb_idx == i);
          }
        }
        SDL_RenderPresent(renderer);
        should_render = 0;
      }
    }
  }

  printf("Done\n");

  text_render_clear();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_StopTextInput();

  // Clean up
  SDL_Quit();
  return 0;
}

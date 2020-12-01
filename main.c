#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "headers/editor_controls.h"
#include "headers/editor_state.h"
#include "headers/editor_view.h"

#include "headers/text_render.h"
#include "headers/types.h"
#include "headers/utils.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600

#define DEFAULT_FONT_PATH "fonts/Inconsolata-Regular.ttf"

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
      .view_props = (EditorViewProps){WIN_WIDTH / 3, WIN_HEIGHT, 0, 0},
      .max_visible_lines = (ui32)(WIN_HEIGHT / tr_props.line_height),
      // index of the top most line visible in editor
      .first_visible_line_idx = 1,
      // min lines rendered after full scroll
      .min_visible_lines_after_scroll = 5,
      .active_line_idx = 1,
      .cursor_pos = 0,
      .main_text_offset_x = 0,
      .main_text_offset_y = 0};
  if (file_path != NULL) {
    FILE *f = fopen(*file_path, "r");
    assert(f);
    editor_init_from_file(&eb_1, f);
    fclose(f);
    f = NULL;
  } else {
    editor_init_empty(&eb_1);
  }

  EditorBuffer eb_2 = eb_1;
  eb_2.view_props.offset_x = WIN_WIDTH / 3;
  EditorBuffer eb_3 = eb_1;
  eb_3.view_props.offset_x = 2 * WIN_WIDTH / 3;

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
        should_render = handle_mouse_button_down(e, &eb_1);
        break;

      case SDL_MOUSEWHEEL:
        should_render = handle_mousewheel(e, &eb_1);
        break;

      case SDL_KEYDOWN:
        should_render = handle_keydown(e, &eb_1);
        break; // SDL_KEYDOWN
      case SDL_TEXTINPUT:
        editor_insert_str_at_cursor(&eb_1, e.text.text);
        should_render = 1;
        break;
      default:
        break;
      } // switch

      if (should_render) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        editor_view_draw(renderer, &eb_1);
        editor_view_draw(renderer, &eb_2);
        editor_view_draw(renderer, &eb_3);
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

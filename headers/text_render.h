#ifndef TEXT_RENDER_H
#define TEXT_RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "text.h"
#include "types.h"

#define ASCII_START 32
#define ASCII_END 126
#define ASCII_PRINTABLE_SIZE (ASCII_END - ASCII_START + 1)

typedef struct TextRenderProps {
  TTF_Font *font;
  ui16 font_size;
  ui16 line_height;
  ui16 glyph_width;
} TextRenderProps;

SDL_Surface *glyph_cache[ASCII_PRINTABLE_SIZE];

SDL_Texture *glyph_cache_textures[ASCII_PRINTABLE_SIZE];

SDL_Texture *get_glyph_texture_from_cache(ui16 ch);

void text_renderer_init(SDL_Renderer *renderer, char *font_path,
                        TextRenderProps *tr_props);

void text_renderer_cache_font(TTF_Font *font, SDL_Renderer *renderer);

void text_renderer_draw_string(SDL_Renderer *renderer,
                               TextRenderProps *tr_props, char *str, i32 x,
                               i32 y, SDL_Color color);

void text_renderer_draw_line(SDL_Renderer *renderer, ui32 max_chars_per_line,
                             TextRenderProps *tr_props, TextLine *line, i32 x,
                             i32 y, SDL_Color color);

void text_renderer_draw_multiline(SDL_Renderer *renderer,
                                  ui32 max_chars_per_line,
                                  TextRenderProps *tr_props,
                                  TextLine *lines_head, i32 from, i32 count,
                                  i32 x, i32 y, SDL_Color color);

void text_render_clear();

#endif

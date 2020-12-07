#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <assert.h>
#include <stdlib.h>

#include "headers/text_render.h"
#include "headers/types.h"

SDL_Surface *glyph_cache_surf[ASCII_PRINTABLE_SIZE];
SDL_Texture *glyph_cache_textures[ASCII_PRINTABLE_SIZE];

void text_renderer_init(SDL_Renderer *renderer, char *font_path,
                        TextRenderProps *tr_props) {
  TTF_Init();
  TTF_Font *font = TTF_OpenFont(font_path, tr_props->font_size);
  assert(font);

  tr_props->font = font;

  TTF_SetFontHinting(font, TTF_HINTING_NORMAL);
  text_renderer_cache_font(font, renderer);
  TTF_SizeText(font, "w", (int *)&(tr_props->glyph_width), NULL);
  TTF_CloseFont(font);
  font = NULL;
}

SDL_Texture *get_glyph_texture_from_cache(ui16 ch) {
  assert(ch >= ASCII_START && ch <= ASCII_END);

  return glyph_cache_textures[ch - ASCII_START];
}

void text_renderer_cache_font(TTF_Font *font, SDL_Renderer *renderer) {
  SDL_Color color = {255, 255, 255};
  SDL_Surface *glyph;
  ui16 ch;
  i32 i;
  for (ch = ASCII_START; ch <= ASCII_END; ++ch) {
    i = ch - ASCII_START;
    glyph = TTF_RenderGlyph_Blended(font, ch, color);
    glyph_cache_surf[i] = glyph;
    glyph_cache_textures[i] = SDL_CreateTextureFromSurface(renderer, glyph);
  }
  glyph = NULL;
}

void text_renderer_draw_string(SDL_Renderer *renderer,
                               TextRenderProps *tr_props, char *str, i32 x,
                               i32 y, SDL_Color color) {
  SDL_Rect dst_rect = {0, 0, 0, 0};
  SDL_Texture *glyph_texture;
  i32 len = strlen(str);
  for (i32 i = 0; i < len; i++) {
    if ((i32)str[i] < ASCII_START || (i32)str[i] > ASCII_END)
      continue;
    i32 w, h;
    glyph_texture = get_glyph_texture_from_cache(str[i]);
    SDL_QueryTexture(glyph_texture, NULL, NULL, &w, &h);
    dst_rect.x = x;
    dst_rect.y = y + (tr_props->line_height - h);
    dst_rect.w = w;
    dst_rect.h = h;
    SDL_SetTextureColorMod(glyph_texture, color.r, color.g, color.b);
    SDL_RenderCopyEx(renderer, glyph_texture, NULL, &dst_rect, 0, NULL,
                     SDL_FLIP_NONE);
    x += dst_rect.w;
    glyph_texture = NULL;
  }
}

void text_renderer_draw_line(SDL_Renderer *renderer, ui32 max_chars_per_line,
                             TextRenderProps *tr_props, TextLine *line, i32 x,
                             i32 y, SDL_Color color) {
  assert(line != NULL);
  char str[TEXT_LINE_MAX_LENGTH] = {0};
  if (max_chars_per_line > TEXT_LINE_MAX_LENGTH) {
    max_chars_per_line = TEXT_LINE_MAX_LENGTH;
  }
  str_copy(str, line->buf, max_chars_per_line);
  text_renderer_draw_string(renderer, tr_props, str, x, y, color);
}

void text_renderer_draw_multiline(SDL_Renderer *renderer,
                                  ui32 max_chars_per_line,
                                  TextRenderProps *tr_props,
                                  TextLine *lines_head, i32 from, i32 count,
                                  i32 x, i32 y, SDL_Color color) {
  TextLine *curr_line = lines_head;
  i32 n = 0;
  i32 to = from + count;
  while (curr_line != NULL && n < to) {
    if (n < from) {
      curr_line = curr_line->next;
    } else {
      text_renderer_draw_line(renderer, max_chars_per_line, tr_props, curr_line,
                              x, y, color);
      y += tr_props->line_height;
      curr_line = curr_line->next;
    }
    n++;
  }
}

void text_render_clear() {
  for (i32 i = 0; i < ASCII_PRINTABLE_SIZE; ++i) {
    SDL_FreeSurface(glyph_cache_surf[i]);
    SDL_DestroyTexture(glyph_cache_textures[i]);
  }
}

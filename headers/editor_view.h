#ifndef EDITOR_VIEW_H
#define EDITOR_VIEW_H

#include "editor_state.h"
#include "types.h"

#define MIN_LINE_NUMBER_PANEL_WIDTH_PX 20

ui32 editor_view_max_chars_per_line(EditorBuffer *eb);
void editor_view_draw(SDL_Renderer *renderer, EditorBuffer *eb,
                      uchar is_active);

#endif
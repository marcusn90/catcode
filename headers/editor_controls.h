#ifndef EDITOR_CONTROLS_H
#define EDITOR_CONTROLS_H

#include "editor_state.h"
#include "types.h"
#include <SDL2/SDL.h>

i32 handle_keydown(SDL_Event e, EditorBuffer *eb);
i32 handle_mousewheel(SDL_Event e, EditorBuffer *eb);
i32 handle_mouse_button_down(SDL_Event e, EditorBuffer *eb);

#endif
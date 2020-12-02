#ifndef EDITOR_CONTROLS_H
#define EDITOR_CONTROLS_H

#include "editor_state.h"
#include "types.h"
#include <SDL2/SDL.h>

i32 editor_controls_handle_keydown(SDL_Event e, EditorBuffer *eb);
i32 editor_controls_handle_mousewheel(SDL_Event e, EditorBuffer *eb);
i32 editor_controls_handle_mouse_button_down(SDL_Event e, EditorBuffer *eb);

#endif
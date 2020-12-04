#ifndef APP_INPUT_HANDLERS_H
#define APP_INPUT_HANDLERS_H

#include "app_state.h"
#include "editor_state.h"
#include "types.h"
#include <SDL2/SDL.h>

i32 app_handle_keydown(SDL_Event e);
i32 app_handle_mousewheel(SDL_Event e);
i32 app_handle_mouse_button_down(SDL_Event e);

#endif
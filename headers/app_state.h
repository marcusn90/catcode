#ifndef APP_STATE_H
#define APP_STATE_H

#include "editor_state.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define MAX_OPEN_EDITOR_BUFFERS 2

typedef struct AppState {
  EditorBuffer *open_editor_buffers[2];
  i32 active_eb_idx;

} AppState;

void app_state_init();
EditorBuffer *app_state_get_active_eb();
void app_state_toggle_split_buffers();
void app_state_set_active_buffer_idx(i32 idx);
#endif
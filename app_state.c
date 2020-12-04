#include "headers/app_state.h"
#include <assert.h>

AppState GAS; // Global App State
EditorBuffer *app_state_get_active_eb() {
  return GAS.open_editor_buffers[GAS.active_eb_idx];
}

void app_state_init() {
  GAS.active_eb_idx = 0;
  GAS.open_editor_buffers[0] = NULL;
  GAS.open_editor_buffers[1] = NULL;
}

void app_state_toggle_split_buffers() {
  EditorBuffer *active_eb = app_state_get_active_eb();
  assert(active_eb);
  if (GAS.open_editor_buffers[1] == NULL) {
    // split screen
    EditorBuffer *new_eb = calloc(1, sizeof(EditorBuffer));
    editor_clone_buffer(active_eb, new_eb);
    GAS.open_editor_buffers[1] = new_eb;
    GAS.open_editor_buffers[0]->view_props.width = WIN_WIDTH / 2;
    GAS.open_editor_buffers[0]->view_props.offset_x = 0;
    GAS.open_editor_buffers[1]->view_props.width = WIN_WIDTH / 2;
    GAS.open_editor_buffers[1]->view_props.offset_x = WIN_WIDTH / 2;
    GAS.active_eb_idx = 1;
  } else {
    // back to single buffer
    active_eb->view_props.width = WIN_WIDTH;
    active_eb->view_props.offset_x = 0;
    GAS.open_editor_buffers[0] = active_eb;
    if (GAS.open_editor_buffers[1] != GAS.open_editor_buffers[0]) {
      free(GAS.open_editor_buffers[1]);
    }
    GAS.open_editor_buffers[1] = NULL;
    GAS.active_eb_idx = 0;
  }
}

void app_state_set_active_buffer_idx(i32 idx) {
  assert(idx >= 0 && idx < 2);
  if (GAS.open_editor_buffers[idx] != NULL) {
    GAS.active_eb_idx = idx;
  }
}
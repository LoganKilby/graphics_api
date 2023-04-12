/* date = April 11th 2023 1:57 pm */

#ifndef INPUT_H
#define INPUT_H

#include "vector_math.h"

enum Input_Device {
    Keyboard,
    Mouse,
    Touch
};

enum Input_Gesture {
    Release,
    Press,
    Scroll,
};

enum Input_Button {
    Mouse_LButton,
    Mouse_RButton,
    Mouse_MButton,
    Mouse_XButton1,
    Mouse_XButton2,
};

#define SysMod_Ctrl 1
#define SysMod_Shift 2
#define SysMod_Alt 4

struct Input_Event {
    Input_Device device;
    Input_Gesture gesture;
    Input_Button button;
    u32 sys_modifiers;
    iv2 screen_position;
};

#define MAX_BUFFERED_INPUT_EVENTS 15
struct Input_Event_List {
    u32 count;
    Input_Event events[MAX_BUFFERED_INPUT_EVENTS];
};

struct Input_State {
    Input_Event_List event_list;
    iv2 mouse_position; // screen coordinates
} global global_input_state;

internal bool push_input_event(Input_Event event) {
    if(global_input_state.event_list.count < countof(global_input_state.event_list.events)) {
        global_input_state.event_list.events[global_input_state.event_list.count++] = event;
        return false;
    } else {
        memcpy(global_input_state.event_list.events, &global_input_state.event_list.events[1], sizeof(Input_Event) * countof(global_input_state.event_list.events) - 1);
        return true;
    }
}

#endif //INPUT_H

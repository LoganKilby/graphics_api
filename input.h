/* date = April 11th 2023 1:57 pm */

#ifndef INPUT_H
#define INPUT_H

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

struct Point {
    int x, y;
};

#define SysMod_Ctrl 1
#define SysMod_Shift 2
#define SysMod_Alt 4

struct Input_Event {
    Input_Device device;
    Input_Gesture gesture;
    Input_Button button;
    u32 sys_modifiers;
    Point screen_position;
};

#define MAX_BUFFERED_INPUT_EVENTS 15
struct Input_Event_List {
    u32 count;
    Input_Event events[MAX_BUFFERED_INPUT_EVENTS];
};

struct Input_State {
    Input_Event_List event_list;
    Point mouse_pos;
    //GLFWwindow *window;
};

internal bool push_input_event(Input_State *state, Input_Event event) {
    if(state->event_list.count < countof(state->event_list.events)) {
        state->event_list.events[state->event_list.count++] = event;
        return false;
    } else {
        // Discarding the oldest input event, then adding to the front of the queue
        memcpy(state->event_list.events, &state->event_list.events[1], sizeof(Input_Event) * (countof(state->event_list.events) - 1));
        state->event_list.events[0] = event;
        assert(0); // an event was discarded...
        return true;
    }
}

#endif //INPUT_H

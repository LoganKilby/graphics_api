/* date = April 11th 2023 1:57 pm */

#ifndef INPUT_H
#define INPUT_H

enum Input_Device_Type {
    Mouse,
    Keyboard
};

struct Input_Event {
    int key;
    int scancode;
    int action;
    int mods;
    Input_Device_Type device;
};

#define MAX_BUFFERED_INPUT_EVENTS 15
struct Input_Event_List {
    u32 count;
    Input_Event events[MAX_BUFFERED_INPUT_EVENTS];
};

struct Input_State {
    v2 mouse_pos;
    v2 mouse_diff;
    f32 mouse_scroll_delta;
    Input_Event_List event_list;
};

internal Input_Event pop_input_event(Input_Event_List *list) {
    Input_Event result = list->events[0];
    memcpy(list->events, &list->events[1], sizeof(Input_Event) * (countof(list->events) - 1));
    list->count = max(0, list->count - 1);
    
    return result;
}

internal void push_input_event(Input_Event_List *event_list, Input_Event event) {
    if(event_list->count < countof(event_list->events)) 
        event_list->events[event_list->count++] = event;
    else
        assert(0); // Input event list full
}

#endif //INPUT_H

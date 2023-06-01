/* date = April 11th 2023 1:57 pm */

#ifndef INPUT_H
#define INPUT_H

struct Input_Event {
    int key;
    int scancode;
    int action;
    int mods;
};

#define MAX_BUFFERED_INPUT_EVENTS 15
struct Input_Event_List {
    u32 count;
    Input_Event events[MAX_BUFFERED_INPUT_EVENTS];
};

internal bool push_input_event(Input_Event_List *event_list, Input_Event event) {
    if(event_list->count < countof(event_list->events)) {
        event_list->events[event_list->count++] = event;
        return false;
    } else {
        // Discarding the oldest input event, then adding to the front of the queue
        memcpy(event_list->events, &event_list->events[1], sizeof(Input_Event) * (countof(event_list->events) - 1));
        event_list->events[0] = event;
        assert(0); // an event was discarded...
        return true;
    }
}

internal bool get_next_input_event(Input_Event_List *list, Input_Event *event) {
    if(list->count)
    {
        *event = list->events[0];
        memcpy(list->events, &list->events[1], sizeof(Input_Event) * (countof(list->events) - 1));
    }
    else
    {
        event = 0;
    }
}

#endif //INPUT_H

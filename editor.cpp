struct Editor_State {
    v4 x_axis_color;
    v4 y_axis_color;
    v4 z_axis_color;
    
    Orbit_Camera camera;
    v3 camera_target;
    
    int hide_key; // keybind
    int active;
};
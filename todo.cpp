// implement release error reporting
// draw custom cursor when not editing
// implement keybinds
// scene file format
// use permanent storage (fonts, etc)
// devise a strategy for generic resource management
// No entities by default. Load them from scene file
// allocate all blob memory before serializing since we know ahead of time how much we're going to write to disk. This removes the need
//   to constantly be calling memmove and realloc
// pull out engine stuff from Application_State

// TRANSIENT ALLOCATE FILE READING STUFF!!!

// COULD BLOB LOADING HAPPEN ON A SEPARATE THREAD? IF SO, WE CAN'T ALLOCATE FROM THE TRANSIENT ARENA

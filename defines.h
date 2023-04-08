/* date = March 27th 2023 9:18 pm */

#ifndef DEFINES_H
#define DEFINES_H

#ifdef DEBUG
#define assert(expression) if(!(expression)) { *(int *)0 = 0; }
#else
#define assert(expression)
#endif


#endif //DEFINES_H

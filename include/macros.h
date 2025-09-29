#ifndef MACROS_H
#define MACROS_H

#define deref_as(TYPE, ITEM) (*((TYPE*)(ITEM)))
#define not !
#define and &&
#define or ||
#define DEBUG_LINE printf("[DEBUG] Reached line: %d\n", __LINE__)

#endif //MACROS_H
#ifndef __SEMAPHORE_STRUCT_H__
#define __SEMAPHORE_STRUCT_H__

struct sembuf acquire_operation = { 0, -1, SEM_UNDO };
struct sembuf release_operation = { 0, 1, SEM_UNDO };

unsigned short init_values[1] = { 1 };

#else

extern struct sembuf acquire_operation;
extern struct sembuf release_operation;

#endif


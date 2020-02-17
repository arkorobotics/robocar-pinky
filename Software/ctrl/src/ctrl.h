#ifndef __CTRL_H__
#define __CTRL_H__

#define DEBUG 1
#define MAX_MISSED_HEARTBEAT 25

extern "C" {
    #include "glue.h"
}

void *ctrl_timer_func(void *);
void sigint(int);
int ctrl_loop(void);
int acquire(int id);
int release(int id);


#endif // __CTRL_H__

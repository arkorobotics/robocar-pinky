#ifndef __CTRL_H__
#define __CTRL_H__

extern "C" {
  #include "glue.h"
}

void *ctrl_timer_func(void *);
void sigint(int);
int ctrl_loop(void);

typedef struct
{
    float steering_pos;
    float drive_vel;
}
Ctrl_Cmd;

typedef struct
{
    uint32_t timestamp;
    Glue_State glue_state;
}
Ctrl_Telem;

#endif // __CTRL_H__

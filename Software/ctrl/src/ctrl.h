#ifndef __CTRL_H__
#define __CTRL_H__

extern "C" {
  #include "glue.h"
}

void *ctrl_timer_func(void *);
void sigint(int);
int ctrl_loop(void);
int acquire(int id);
int release(int id);

struct Ctrl_Cmd
{
    float steering_pos;
    float drive_vel;
} __attribute__((packed));

struct Ctrl_Telem
{
    uint32_t timestamp;
    Glue_State glue_state;
} __attribute__((packed));

#endif // __CTRL_H__

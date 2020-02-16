#ifndef __COMM_H__
#define __COMM_H__

extern "C" {
  #include "glue.h"
}

int comm_init(Ctrl_Cmd *shared_ctrl_cmd, Ctrl_Telem *shared_ctrl_telem);
void comm_sem_acquire(void);
void comm_sem_release(void);
int sem_acquire(int id);
int sem_release(int id);

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
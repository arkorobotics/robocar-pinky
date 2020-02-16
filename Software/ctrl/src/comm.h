#ifndef __COMM_H__
#define __COMM_H__

extern "C" {
  #include "glue.h"
}

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

int comm_init(void);
void comm_close(void);
void comm_transaction(Ctrl_Cmd *ctrl_cmd, Ctrl_Telem *ctrl_telem);
int sem_acquire(int id);
int sem_release(int id);

#endif // __CTRL_H__

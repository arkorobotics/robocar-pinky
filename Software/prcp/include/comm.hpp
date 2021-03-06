#ifndef __COMM_H__
#define __COMM_H__

enum Ctrl_Mode {
    ESTOP,
    INIT,
    IDLE,
    RUN,
    STOP,
    FAULT,
    CLEARFAULT
};

struct Glue_State
{
    uint32_t vmotor_battery_mv;
    uint32_t vlogic_battery_mv;
    float steer_position;
    float drive_torque;
    float drive_velocity;
} __attribute__((packed));

struct Ctrl_Cmd
{
    uint32_t mode;
    uint32_t heartbeat;
    float steer_pos;
    float drive_vel;
} __attribute__((packed));

struct Ctrl_Telem
{
    uint32_t mode;
    int64_t tosleep;
    uint32_t timestamp;
    Glue_State glue_state;
} __attribute__((packed));

int comm_init(void);
void comm_close(void);
void comm_prcp_transaction(Ctrl_Cmd *ctrl_cmd, Ctrl_Telem *ctrl_telem);
int sem_acquire(int id);
int sem_release(int id);

#endif // __CTRL_H__

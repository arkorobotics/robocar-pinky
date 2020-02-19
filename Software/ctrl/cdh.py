import sysv_ipc
from struct import *
from time import sleep
from enum import IntEnum

# Shared memory and semaphone variables
cmd_key = 1000                   # CMD shared memory key
cmd_sem_key = 1001               # CMD semaphore key

telem_key = 2000                 # TELEM shared memory key
telem_sem_key = 2001             # TELEM semaphore key

# CTRL state machine ENUM
class Mode(IntEnum):
    ESTOP=0
    INIT=1
    IDLE=2
    RUN=3
    STOP=4
    FAULT=5
    CLEARFAULT=6

# Create shared memory object
cmd_memory = sysv_ipc.SharedMemory(1000)
cmd_sem = sysv_ipc.Semaphore(1001)

telem_memory = sysv_ipc.SharedMemory(2000)
telem_sem = sysv_ipc.Semaphore(2001)

# CTRL Command Variables
mode = int(Mode.CLEARFAULT)
heartbeat = 0
cmd_steer_pos = 0.0
cmd_drive_vel = 0.0

# Intialize command packet
cmd_packet = pack('=LLff', mode, heartbeat, cmd_steer_pos, cmd_drive_vel)

# Main Loop
while(True):

    # Lock semaphore, read/write to CTRL memory, and unlock semaphore
    telem_sem.acquire()
    cmd_sem.acquire()

    telem_packet = telem_memory.read()
    cmd_memory.write(cmd_packet)
    cmd_data_ro = cmd_memory.read()

    telem_sem.release()
    cmd_sem.release()

    # Unpack telemetry and read-back command data
    telem_data = unpack('=LqLLLfff', telem_packet)
    cmd_data_ro = unpack('=LLff', cmd_data_ro)

    # ==========
    # prcp loop?
    # ==========

    # C&DH
    
    # Generate new command packet
    cmd_packet = pack('=LLff', int(Mode.ESTOP), heartbeat, 2, 3)

    # Print 
    print(telem_data)
    print(cmd_data_ro)

    # Increment Heartbeat Count
    heartbeat = heartbeat + 1

    # Sleep for 20ms to simulate future vision processing load
    sleep(0.02)

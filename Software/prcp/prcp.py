import sysv_ipc
from struct import *
from time import sleep
from enum import IntEnum

import pyzed.sl as sl
import math
import numpy as np
import sys
import os

# Get the number of CPUs
# in the system
# using os.cpu_count() method
print("Number of CPUs:", os.cpu_count())

# Get the set of CPUs
# on which the calling process
# is eligible to run. using
# os.sched_getaffinity() method
# 0 as PID represnts the
# calling process
pid = 0
affinity = os.sched_getaffinity(pid)

# Print the result
print("Process is eligibl to run on:", affinity)

# Change the CPU affinity mask
# of the calling process
# using os.sched_setaffinity() method

# Below CPU affinity mask will
# restrict a process to only
# these 2 CPUs (0, 1) i.e process can
# run on these CPUs only
affinity_mask = {1}
pid = 0
os.sched_setaffinity(0, affinity_mask)
print("CPU affinity mask is modified for process id % s" % pid)


# Now again, Get the set of CPUs
# on which the calling process
# is eligible to run.
pid = 0
affinity = os.sched_getaffinity(pid)

# Print the result
print("Now, process is eligibl to run on:", affinity)

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

# Create a Camera object
zed = sl.Camera()

# Create a InitParameters object and set configuration parameters
init_params = sl.InitParameters()
init_params.depth_mode = sl.DEPTH_MODE.PERFORMANCE  # Use PERFORMANCE depth mode
init_params.coordinate_units = sl.UNIT.MILLIMETER  # Use milliliter units (for depth measurements)

# Open the camera
err = zed.open(init_params)
if err != sl.ERROR_CODE.SUCCESS:
    exit(1)

# Create and set RuntimeParameters after opening the camera
runtime_parameters = sl.RuntimeParameters()
runtime_parameters.sensing_mode = sl.SENSING_MODE.STANDARD  # Use STANDARD sensing mode

# Capture 50 images and depth, then stop
i = 0
image = sl.Mat()
depth = sl.Mat()
point_cloud = sl.Mat()

mirror_ref = sl.Transform()
mirror_ref.set_translation(sl.Translation(2.75,4.0,0))
tr_np = mirror_ref.m

# Main Loop
while(True):

    # Lock semaphore, read/write to CTRL memory, and unlock semaphore
    #telem_sem.acquire()
    #cmd_sem.acquire()

    #telem_packet = telem_memory.read()
    #cmd_memory.write(cmd_packet)
    #cmd_data_ro = cmd_memory.read()

    #telem_sem.release()
    #cmd_sem.release()

    # Unpack telemetry and read-back command data
    #telem_data = unpack('=LqLLLfff', telem_packet)
    #cmd_data_ro = unpack('=LLff', cmd_data_ro)

    # Generate new command packet
    cmd_packet = pack('=LLff', int(Mode.RUN), heartbeat, 0.0, 0.2)


    # ==========
    # prcp loop
    # ==========


    # A new image is available if grab() returns SUCCESS
    if zed.grab(runtime_parameters) == sl.ERROR_CODE.SUCCESS:
        # Retrieve left image
        zed.retrieve_image(image, sl.VIEW.LEFT)
        # Retrieve depth map. Depth is aligned on the left image
        zed.retrieve_measure(depth, sl.MEASURE.DEPTH)
        # Retrieve colored point cloud. Point cloud is aligned on the left image.
        zed.retrieve_measure(point_cloud, sl.MEASURE.XYZRGBA)

        # Get and print distance value in mm at the center of the image
        # We measure the distance camera - object using Euclidean distance
        x = round(image.get_width() / 2)
        y = round(image.get_height() / 2)
        err, point_cloud_value = point_cloud.get_value(x, y)

        distance = math.sqrt(point_cloud_value[0] * point_cloud_value[0] +
                             point_cloud_value[1] * point_cloud_value[1] +
                             point_cloud_value[2] * point_cloud_value[2])

        point_cloud_np = point_cloud.get_data()
        point_cloud_np.dot(tr_np)

        if not np.isnan(distance) and not np.isinf(distance):
            distance = round(distance)
            print("Distance to Camera at ({0}, {1}): {2} mm\n".format(x, y, distance))
            # Increment the loop
            i = i + 1
        else:
            print("Can't estimate distance at this position, move the camera\n")
            sys.stdout.flush()

    # Print
    #print(telem_data)
    #print(cmd_data_ro)

    # Increment Heartbeat Count
    heartbeat = heartbeat + 1

    # Sleep for 20ms to simulate future vision processing load
    sleep(0.02)


# Close the camera
zed.close()

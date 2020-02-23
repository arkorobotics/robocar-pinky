from struct import *
from time import sleep
from enum import IntEnum

import os
import sys
import sysv_ipc
import math
import cv2
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt
import pyzed.sl as sl

np.set_printoptions(threshold=sys.maxsize)

img_height = 720
img_width = 1280

window_left = 0
window_right = 1280
window_top = 300
window_bottom = 600

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
#depth = sl.Mat()
point_cloud = sl.Mat()

# Create an RGBA sl.Mat object
image_zed = sl.Mat(zed.get_camera_information().camera_resolution.width, zed.get_camera_information().camera_resolution.height, sl.MAT_TYPE.U8_C4)

# Retrieve data in a numpy array with get_data()
image_ocv = image_zed.get_data()

# Create a sl.Mat with float type (32-bit)
depth = sl.Mat(zed.get_camera_information().camera_resolution.width, zed.get_camera_information().camera_resolution.height, sl.MAT_TYPE.F32_C1)


mirror_ref = sl.Transform()
mirror_ref.set_translation(sl.Translation(2.75,4.0,0))
tr_np = mirror_ref.m


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

        
        # Convert BGR to HSV
        image = cv2.medianBlur(image,5)
        hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

        # define range of blue color in HSV
        lower_yellow = np.array([20,80,80])
        upper_yellow = np.array([46,255,255])

        # Threshold the HSV image to get only blue colors
        mask = cv2.inRange(hsv, lower_yellow, upper_yellow)

        window_histo = np.zeros(window_right - window_left)

        for col in range(window_left, window_right):
            for row in range(window_top, window_bottom):
                window_histo[col] += mask[row,col]

        mean = np.int(np.average(np.arange(window_histo.size), weights=window_histo))
        print(mean)

        # Bitwise-AND mask and original image
        res = cv2.bitwise_and(image,image, mask= mask)
        cv2.line(res,(mean,window_top),(mean,window_bottom),(0,0,255),5)

        cv2.imshow('frame',image)
        #cv2.imshow('mask',mask)
        cv2.imshow('res',res)
        cv2.waitKey()
        cv2.destroyAllWindows()

        image_depth_ocv = depth.get_data()
        cv2.imshow("Image", image_depth_ocv)
        cv2.waitKey()

    # Generate new command packet
    cmd_packet = pack('=LLff', int(Mode.RUN), heartbeat, 0.0, 0.2)

    # Print
    print(telem_data)
    print(cmd_data_ro)

    # Increment Heartbeat Count
    heartbeat = heartbeat + 1

    # Sleep for 20ms to simulate future vision processing load
    sleep(0.02)


# Close the camera
zed.close()

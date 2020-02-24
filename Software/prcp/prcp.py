from struct import *
from time import sleep
from enum import IntEnum

import os
import sys
import sysv_ipc
import math
import cv2
import numpy as np
#import matplotlib
#import matplotlib.mlab as mlab
#import matplotlib.pyplot as plt
import pyzed.sl as sl

#matplotlib.use('qt4agg')

np.set_printoptions(threshold=sys.maxsize)

img_height = 376
img_width = 672

window_left = 0
window_right = 672
window_center = np.int((window_right - window_left)/2)

window_top = 200
window_bottom = 300

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

# CTRL calc variabls
cmd_steer_max = 0.5

# Intialize command packet
cmd_packet = pack('=LLff', mode, heartbeat, cmd_steer_pos, cmd_drive_vel)

# Create a Camera object
zed = sl.Camera()

# Create a InitParameters object and set configuration parameters
init_params = sl.InitParameters()
init_params.depth_mode = sl.DEPTH_MODE.PERFORMANCE  # Use PERFORMANCE depth mode
init_params.coordinate_units = sl.UNIT.MILLIMETER  # Use milliliter units (for depth measurements)
# Set configuration parameters
init_params.camera_resolution = sl.RESOLUTION.VGA
init_params.camera_fps = 100

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
        #zed.retrieve_measure(point_cloud, sl.MEASURE.XYZRGBA)

        img = image.get_data()

        # Convert BGR to HSV
        img = cv2.medianBlur(img,5)
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)

        # define range of blue color in HSV
        lower_yellow = np.array([20,60,60])
        upper_yellow = np.array([46,255,255])

        # Threshold the HSV image to get only blue colors
        mask = cv2.inRange(hsv, lower_yellow, upper_yellow)

        window_histo = np.ones(window_right - window_left)

        for col in range(window_left, window_right):
            for row in range(window_top, window_bottom):
                window_histo[col] += mask[row,col]

        mean = np.int(np.average(np.arange(window_histo.size), weights=window_histo))
        print(mean)

        # Bitwise-AND mask and original image
        res = cv2.bitwise_and(img,img, mask= mask)
        cv2.line(res,(mean,window_top),(mean,window_bottom),(0,0,255),5)

        cv2.imshow('road-o-vision',res)

#        image_depth_ocv = depth.get_data()
#        cv2.imshow('depth', image_depth_ocv)

        cv2.waitKey(1)

    # Calculate CMD variables
    cmd_steer_pos = -1.0*cmd_steer_max*(mean - window_center)/((window_right - window_left)/2)
    cmd_drive_vel = 0.1

    # Generate new command packet
    cmd_packet = pack('=LLff', int(Mode.RUN), heartbeat, cmd_steer_pos, cmd_drive_vel)

    # Print
    print(telem_data)
    print(cmd_data_ro)

    # Increment Heartbeat Count
    heartbeat = heartbeat + 1

cv2.destroyAllWindows()

# Close the camera
zed.close()

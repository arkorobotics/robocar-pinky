# Preception Testbed
import sys
import cv2
import numpy as np
import matplotlib.mlab as mlab
import matplotlib.pyplot as plt

np.set_printoptions(threshold=sys.maxsize)

img_height = 720
img_width = 1280

window_left = 0
window_right = 1280
window_top = 300
window_bottom = 600

img = cv2.imread('track_above.jpg')

# Convert BGR to HSV
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
img = cv2.medianBlur(img,5)

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
res = cv2.bitwise_and(img,img, mask= mask)
cv2.line(res,(mean,window_top),(mean,window_bottom),(0,0,255),5)

#cv2.imshow('frame',img)
#cv2.imshow('mask',mask)
cv2.imshow('res',res)
cv2.waitKey()
cv2.destroyAllWindows()

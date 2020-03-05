#ifndef __PRCP_H__
#define __PRCP_H__

#define DRIVE_REV_MAX_VAL   2.600
#define DRIVE_ZERO_VAL      1.400
#define DRIVE_FWD_MAX_VAL   0.300
#define DRIVE_SCALER        (DRIVE_REV_MAX_VAL - DRIVE_FWD_MAX_VAL)/2

#define IMG_HEIGHT 188
#define IMG_WIDTH 336

#define WINDOW_LEFT 0
#define WINDOW_RIGHT 336
#define WINDOW_WIDTH (WINDOW_RIGHT - WINDOW_LEFT)
#define WINDOW_CENTER (WINDOW_WIDTH / 2)

#define WINDOW_TOP 130
#define WINDOW_BOTTOM 175
#define WINDOW_HEIGHT (WINDOW_BOTTOM - WINDOW_TOP)

#define H_LOW   20
#define H_HIGH  35

#define S_LOW   110
#define S_HIGH  255

#define V_LOW   110
#define V_HIGH  255

#define LANE_N  3

#define DRIVE_VEL_MAX       0.005
#define DRIVE_VEL_STR_GAIN  2000.0

#define CMD_STEER_MAX       1.5

void parseArgs(int argc, char **argv,sl::InitParameters& param);
cv::Mat slMat2cvMat(sl::Mat& input);
float weightedMean(uint32_t *X, uint32_t *W, uint32_t n);
void sigint(int);

#endif // __PRCP_H__

#ifndef __PRCP_H__
#define __PRCP_H__

#define DRIVE_REV_MAX_VAL   2.600
#define DRIVE_ZERO_VAL      1.400
#define DRIVE_FWD_MAX_VAL   0.300
#define DRIVE_SCALER        (DRIVE_REV_MAX_VAL - DRIVE_FWD_MAX_VAL)/2

#define DRIVE_VEL_STR_GAIN  2000.0

void parseArgs(int argc, char **argv,sl::InitParameters& param);
cv::Mat slMat2cvMat(sl::Mat& input);
float weightedMean(uint32_t *X, uint32_t *W, uint32_t n);
void sigint(int);

#endif // __PRCP_H__

#ifndef __PRCP_H__
#define __PRCP_H__

void parseArgs(int argc, char **argv,sl::InitParameters& param);
cv::Mat slMat2cvMat(sl::Mat& input);
float weightedMean(uint32_t *X, uint32_t *W, uint32_t n);
void sigint(int);

#endif // __PRCP_H__

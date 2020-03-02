#ifndef __PRCP_H__
#define __PRCP_H__

void parseArgs(int argc, char **argv,sl::InitParameters& param);
cv::Mat slMat2cvMat(sl::Mat& input);
float weightedMean(int X[], int W[], int n);
void sigint(int);

#endif // __CTRL_H__

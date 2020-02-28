#ifndef __PRCP_H__
#define __PRCP_H__

void parseArgs(int argc, char **argv,sl::InitParameters& param);
cv::Mat slMat2cvMat(sl::Mat& input);
void sigint(int);

#endif // __CTRL_H__

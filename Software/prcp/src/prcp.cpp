/**************************************************************************/
/*!
    @file     prcp.cpp
    @author   Arko
    @license  BSD (see license.txt)
    Robocar Preception Program
    v1.0 - First release
*/
/**************************************************************************/

#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <errno.h>
#include <math.h>

// ZED includes
#include <sl/Camera.hpp>

// OpenCV includes
#include <opencv2/opencv.hpp>

#include "prcp.hpp"
#include "comm.hpp"
#include "glviewer.hpp"

// Using std and sl namespaces
using namespace std;
using namespace sl;

// Local Communication Variables
Ctrl_Cmd ctrl_cmd;                              // Command data from C&DH
Ctrl_Telem ctrl_telem;                          // Telemetry data to C&DH

// Local Variables
bool volatile prcp_run = true;                  // Perception loop run state

uint32_t window_index_array[WINDOW_WIDTH];      // Index array for weightedMean
uint32_t window_histo_array[WINDOW_WIDTH];      // Index histogram array for weightedMean

uint32_t lane_x[LANE_N];                        // Lane X-Axis Array
uint32_t lane_y[LANE_N];                        // Lane Y-Axis Array
uint32_t lane_average = 0;                      // Lane Average value for X-Axis scanning window height
int32_t lane_drive_index = LANE_N;              // Index for which lane segment to follow (currently unused)

float drive_vel_mea = 0.0;                      // Measured drive velocity

/**************************************************************************/
/*!
    @brief  Perception program main
    @param  argc    Arguement count
    @param  argv    Arguement variables
    @return Error code
*/
/**************************************************************************/
int main(int argc, char **argv)
{
    // ========================================================================
    // Initalize perception system variables
    // ========================================================================
    // Make sure ctrl-C stops the program under controlled circumstances
    signal(SIGINT, &sigint);

    // CTRL Command Variables
    ctrl_cmd.mode = CLEARFAULT;
    ctrl_cmd.heartbeat = 0;
    ctrl_cmd.steer_pos = 0.0;
    ctrl_cmd.drive_vel = 0.0;

    // PRCP Telem Variables
    ctrl_telem.mode = FAULT;

    // Initalize window index array for perception heuristics
    for(int i = 0; i < window_width; i++)
    {
        window_index_array[i] = i;
    }
    // ========================================================================

    // ========================================================================
    // Initalize Shared Memory
    // ========================================================================
    print("Starting Perception System...\n");

    // Acquire Shared Memory and Semaphore
    if(comm_init() != 0)
    {
        return(0);
    }

    // Clear Fault since ctrl runs first
    comm_prcp_transaction(&ctrl_cmd, &ctrl_telem);
    // ========================================================================

    // ========================================================================
    // Initalize ZED Mini and Image Processing
    // ========================================================================
    // Configure ZED Mini Stereo Camera + IMU
    Camera zed;

    // Set configuration parameters for the ZED Camera
    InitParameters initParameters;
    initParameters.camera_resolution = RESOLUTION::VGA;
    initParameters.camera_fps = 100;
    initParameters.depth_mode = DEPTH_MODE::PERFORMANCE;
    initParameters.coordinate_system = COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP; // OpenGL's coordinate system is right_handed
    initParameters.coordinate_units = UNIT::METER;
    initParameters.sensors_required = true;
    parseArgs(argc,argv,initParameters);

    // Open the camera and handle any errors
    print("Opening connection to camera...\n");
    ERROR_CODE zed_error = zed.open(initParameters);

    if (zed_error != ERROR_CODE::SUCCESS) {// Quit if an error occurred
        print("Opening camera failed: ",zed_error);
        zed.close();
        return 1;
    }

    // Set runtime parameters after opening the camera
    RuntimeParameters runtime_parameters;
    runtime_parameters.sensing_mode = SENSING_MODE::FILL;

    // Set parameters for Positional Tracking
    PositionalTrackingParameters positional_tracking_param;
    positional_tracking_param.enable_area_memory = true;

    // Enable Positional Tracking
    zed_error = zed.enablePositionalTracking(positional_tracking_param);
    if (zed_error != ERROR_CODE::SUCCESS) {
        print("Enabling positionnal tracking failed: ", zed_error);
        zed.close();
        return 1; // Quit if an error occurred
    }

    // ZED Mini Pose Variables
    Pose zed_pose;
    POSITIONAL_TRACKING_STATE tracking_state;

    // Prepare new image size to retrieve half-resolution images
    Resolution image_size = zed.getCameraInformation().camera_resolution;
    int image_width = image_size.width / 2;
    int image_height = image_size.height / 2;
    Resolution new_image_size(image_width, image_height);

    // To share data between sl::Mat and cv::Mat for BGR image
    // Only the headers and pointer to the sl::Mat are copied, not the data itself
    Mat image_zed(image_width, image_height, MAT_TYPE::U8_C4);

    // Create an OpenCV Mat that shares sl::Mat data
    cv::Mat image_ocv = slMat2cvMat(image_zed);

    // Create an OpenCV Mat for HSV image
    cv::Mat image_hsv = cv::Mat::zeros(image_width, image_height, CV_8UC3);

    // Create an OpenCV Mat for lane color mask
    cv::Mat image_mask = cv::Mat::zeros(image_width, image_height, CV_8U);

    // Create an OpenCV Mat for perception system visualizer
    cv::Mat image_prcp = cv::Mat::zeros(image_width, image_height, CV_8UC3);

    // Create shared data between sl::Mat and cv::Mat for depth image
    // Only the headers and pointer to the sl::Mat are copied, not the data itself
    Mat depth_image_zed(image_width, image_height, MAT_TYPE::U8_C4);
    cv::Mat depth_image_ocv = slMat2cvMat(depth_image_zed);
    // ========================================================================


    // ========================================================================
    // Perception System Main Loop
    // ========================================================================
    print("Starting Main Loop...\n");

    while (prcp_run)
    {
        // Clear faults and run
        if(ctrl_telem.mode == FAULT)
        {
            ctrl_cmd.mode = CLEARFAULT;
        }
        else
        {
            ctrl_cmd.mode = RUN;
        }

        // ========================================================================
        // Acquire and process perception data
        // ========================================================================
        if (zed.grab() == ERROR_CODE::SUCCESS)
        {
            // Get latest pose from ZED Mini
            tracking_state = zed.getPosition(zed_pose, REFERENCE_FRAME::CAMERA);

            // Retrieve the left image in sl::Mat, the cv::Mat is automatically updated
            zed.retrieveImage(image_zed, VIEW::LEFT, MEM::CPU, new_image_size);
            zed.retrieveImage(depth_image_zed, VIEW::DEPTH, MEM::CPU, new_image_size);

            // ========================================================================
            // Lane following heuristics
            // ========================================================================

            // Blank perception visualizer
            image_prcp = cv::Mat::zeros(image_width, image_height, CV_8UC3);

            // Convert ZED image to hsv color space
            cv::cvtColor(image_ocv, image_hsv, cv::COLOR_BGR2HSV);

            // Detect the yellow colored lanes using HSV range values for threshold
            inRange(image_hsv, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), image_mask);

            // Bitwise-AND mask the original image
            cv::bitwise_and(image_ocv, image_ocv, image_prcp, image_mask);

            // Iterate through the segmented rows of the scan window and find the weighted mean of each of them
            for (uint32_t lane_i = 0; lane_i < LANE_N; lane_i++)
            {
                lane_y[lane_i] = (WINDOW_HEIGHT/LANE_N)*lane_i + WINDOW_TOP;
                lane_y[0] = WINDOW_TOP;

                for (uint32_t col = WINDOW_LEFT; col < WINDOW_RIGHT; col++)
                {
                    window_histo_array[col] = 0;

                    for (uint32_t row = lane_y[lane_i]; row < lane_y[lane_i]+(WINDOW_HEIGHT/LANE_N); row++)
                    {
                        window_histo_array[col] += (uint32_t)image_mask.at<uchar>(row,col);
                    }
                }

                // Compute the weighted mean of the segmented row
                lane_x[lane_i] = (int32_t)weightedMean(window_index_array, window_histo_array, window_width);

                // If the mean is 0, no lanes were found so steer forward
                if(lane_x[lane_i] == 0)
                {
                    lane_x[lane_i] = WINDOW_CENTER;
                }
            }

            // Calculate forward drive velocity
            drive_vel_mea = -1.0*zed_pose.getTranslation().tz;

            // Average each row segment
            for(uint32_t s = 0; s < LANE_N; s++)
            {
                lane_average += (uint32_t)( (float)lane_x[s] / 3.0);
            }

            // Steer harder when driving faster
            if(lane_average >= WINDOW_CENTER)
            {
                lane_average = (uint32_t)( (float)lane_average + DRIVE_VEL_MEA_STR_GAIN*drive_vel_mea);
            }
            else
            {
                lane_average = (uint32_t)( (float)lane_average - DRIVE_VEL_MEA_STR_GAIN*drive_vel_mea);
            }

            // For visualization, calculate how far ahead to look and steer against
            // and draw a circle around the index of that lane marker. Current algorithm
            // does not use this variable to steer, but it's useful to plot for visualization.
            lane_drive_index = (int32_t)LANE_N - (int32_t)((float)LANE_N*(drive_vel_mea/DRIVE_VEL_MEA_MAX));

            if(lane_drive_index >= (int32_t)LANE_N)
            {
                lane_drive_index = (int32_t)LANE_N-1;
            }
            if(lane_drive_index < 0)
            {
                lane_drive_index = 0;
            }

            // Draw an outline of the lane in red
            for (uint32_t lane_k = 0; lane_k < LANE_N-1; lane_k++)
            {
                if(lane_k == lane_drive_index)
                {
                    cv::line(image_prcp,cv::Point(lane_x[lane_k],lane_y[lane_k]),cv::Point(lane_x[lane_k+1],lane_y[lane_k+1]),cv::Scalar(0,255,0),5);
                }
                else
                {
                    cv::line(image_prcp,cv::Point(lane_x[lane_k],lane_y[lane_k]),cv::Point(lane_x[lane_k+1],lane_y[lane_k+1]),cv::Scalar(0,0,255),5);
                }
            }

            // Draw a green circle around the 'look ahead' distance along the lane
            cv::circle(image_prcp,cv::Point(lane_x[lane_drive_index],lane_y[lane_drive_index]),5,cv::Scalar(0,255,0),3,8,0);

            // Draw a blue line at the average of all the lanes
            cv::line(image_prcp,cv::Point(lane_average,window_top),cv::Point(lane_average,window_bottom),cv::Scalar(255,0,0),5);

            // Overlay the depth map and lane visualizer
            double alpha = 0.7; double beta;
            beta = ( 1.0 - alpha );
            cv::addWeighted(image_prcp, alpha, depth_image_ocv, beta, 0.0, image_prcp);
            // ========================================================================

            // Display the processed image showing lane following and depth
            cv::imshow("Res", image_prcp);
            cv::waitKey(1);
        }
        else
        {
            sleep_ms(1);
            print("Failed to grab frame!\n");
        }
        // ========================================================================

        // ========================================================================
        // Update and send control command variables
        // ========================================================================
        // Incremement the heartbeat
        ctrl_cmd.heartbeat++;

        // Steer the robocar using the lane_average (not the lane_drive_index which doesn't work well)
        ctrl_cmd.steer_pos = -1.0*CMD_STEER_MAX*(lane_average - WINDOW_CENTER)/((WINDOW_RIGHT - WINDOW_LEFT)/2);
        ctrl_cmd.drive_vel = DRIVE_VEL;

        // Print results
        cout << ", drv vel = " << drive_vel_mea << ", drv line index = " << lane_drive_index;
        cout << ", line mid = " << lane_x[lane_drive_index] << ", steer_pos = " << ctrl_cmd.steer_pos << endl;
        
        // Send/Receive the latest telemetry/command data from shared memory to local memory
        comm_prcp_transaction(&ctrl_cmd, &ctrl_telem);
        // ========================================================================
    }

    // ========================================================================
    // Close perception system
    // ========================================================================
    ctrl_cmd.mode = ESTOP;
    ctrl_cmd.heartbeat = 0;
    ctrl_cmd.steer_pos = 0.0;
    ctrl_cmd.drive_vel = 0.0;
    comm_prcp_transaction(&ctrl_cmd, &ctrl_telem);

    print("Shuting down prcp...\n");

    // Close OpenCV windows
    cv::destroyAllWindows();

    // close the ZED
    zed.disablePositionalTracking();
    zed.close();
    // ========================================================================

    return 0;
}

/**************************************************************************/
/*!
    @brief  Parse program arguments
*/
/**************************************************************************/
void parseArgs(int argc, char **argv,sl::InitParameters& param)
{
    if (argc > 1 && string(argv[1]).find(".svo")!=string::npos) {
        // SVO input mode
        param.input.setFromSVOFile(argv[1]);
        cout<<"[Sample] Using SVO File input: "<<argv[1]<<endl;
    } else if (argc > 1 && string(argv[1]).find(".svo")==string::npos) {
        string arg = string(argv[1]);
        unsigned int a,b,c,d,port;
        if (sscanf(arg.c_str(),"%u.%u.%u.%u:%d", &a, &b, &c, &d,&port) == 5) {
            // Stream input mode - IP + port
            string ip_adress = to_string(a)+"."+to_string(b)+"."+to_string(c)+"."+to_string(d);
            param.input.setFromStream(sl::String(ip_adress.c_str()),port);
            cout<<"[Sample] Using Stream input, IP : "<<ip_adress<<", port : "<<port<<endl;
        }
        else  if (sscanf(arg.c_str(),"%u.%u.%u.%u", &a, &b, &c, &d) == 4) {
            // Stream input mode - IP only
            param.input.setFromStream(sl::String(argv[1]));
            cout<<"[Sample] Using Stream input, IP : "<<argv[1]<<endl;
        }
        else if (arg.find("HD2K")!=string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD2K;
            cout<<"[Sample] Using Camera in resolution HD2K"<<endl;
        } else if (arg.find("HD1080")!=string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD1080;
            cout<<"[Sample] Using Camera in resolution HD1080"<<endl;
        } else if (arg.find("HD720")!=string::npos) {
            param.camera_resolution = sl::RESOLUTION::HD720;
            cout<<"[Sample] Using Camera in resolution HD720"<<endl;
        } else if (arg.find("VGA")!=string::npos) {
            param.camera_resolution = sl::RESOLUTION::VGA;
            cout<<"[Sample] Using Camera in resolution VGA"<<endl;
        }
    } else {
        // Default
    }
}

/**************************************************************************/
/*!
    @brief  Conversion function between sl::Mat and cv::Mat
*/
/**************************************************************************/
cv::Mat slMat2cvMat(sl::Mat& input) {
    // Mapping between MAT_TYPE and CV_TYPE
    int cv_type = -1;
    switch (input.getDataType()) {
        case MAT_TYPE::F32_C1: cv_type = CV_32FC1; break;
        case MAT_TYPE::F32_C2: cv_type = CV_32FC2; break;
        case MAT_TYPE::F32_C3: cv_type = CV_32FC3; break;
        case MAT_TYPE::F32_C4: cv_type = CV_32FC4; break;
        case MAT_TYPE::U8_C1: cv_type = CV_8UC1; break;
        case MAT_TYPE::U8_C2: cv_type = CV_8UC2; break;
        case MAT_TYPE::U8_C3: cv_type = CV_8UC3; break;
        case MAT_TYPE::U8_C4: cv_type = CV_8UC4; break;
        default: break;
    }

    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(MEM::CPU));
}

/**************************************************************************/
/*!
    @brief  Function to calculate weighted mean
*/
/**************************************************************************/
float weightedMean(uint32_t *X, uint32_t *W, uint32_t n)
{
    uint32_t sum = 0, numWeight = 0;

    for (uint32_t i = 0; i < n; i++)
    {
        numWeight = numWeight + X[i] * W[i];
        sum = sum + W[i];
    }

    return (float)numWeight / sum;
}

/**************************************************************************/
/*!
    @brief  Signal interrupt to kill real-time control thread
*/
/**************************************************************************/
void sigint(int)
{
    prcp_run = false;
}




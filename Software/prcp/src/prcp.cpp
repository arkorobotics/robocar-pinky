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

// Local Variables
bool volatile prcp_run = true;                  // Preception loop run state

const uint32_t img_height = 376 / 2;
const uint32_t img_width = 672 / 2;

const uint32_t window_left = 0;
const uint32_t window_right = 672 / 2;
const uint32_t window_center = (window_right - window_left)/2;

const uint32_t window_top = 220 / 2;
const uint32_t window_bottom = 320 / 2;

const int low_H = 20, low_S = 70, low_V = 70;
const int high_H = 46, high_S = 255, high_V = 255;

// Local Communication Variables
Ctrl_Cmd ctrl_cmd;                              // Command data from C&DH
Ctrl_Telem ctrl_telem;                          // Telemetry data to C&DH

int main(int argc, char **argv)
{
    // Make sure ctrl-C stops the program under controlled circumstances
    signal(SIGINT, &sigint);

    // CTRL Command Variables
    ctrl_cmd.mode = CLEARFAULT;
    ctrl_cmd.heartbeat = 0;
    ctrl_cmd.steer_pos = 0.0;
    ctrl_cmd.drive_vel = 0.0;

    // PRCP Telem Variables
    ctrl_telem.mode = FAULT;

    // CTRL calc variabls
    float cmd_steer_max = 1.5;

    print("Starting Perception System...\n");

    // Acquire Shared Memory and Semaphore
    if(comm_init() != 0)
    {
        return(0);
    }

    // Clear Fault since ctrl runs first
    comm_prcp_transaction(&ctrl_cmd, &ctrl_telem);

    Camera zed;

    // Set configuration parameters for the ZED
    InitParameters initParameters;
    initParameters.camera_resolution = RESOLUTION::VGA;
    initParameters.camera_fps = 100;
    initParameters.depth_mode = DEPTH_MODE::PERFORMANCE;
    initParameters.coordinate_system = COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP; // OpenGL's coordinate system is right_handed
    initParameters.coordinate_units = UNIT::METER;
    parseArgs(argc,argv,initParameters);

    // Open the camera
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

    // Prepare new image size to retrieve half-resolution images
    Resolution image_size = zed.getCameraInformation().camera_resolution;
    int new_width = image_size.width / 2;
    int new_height = image_size.height / 2;

    Resolution new_image_size(new_width, new_height);

    // To share data between sl::Mat and cv::Mat, use slMat2cvMat()    // Only the headers and pointer to the sl::Mat are copied, not the data itself
    Mat image_zed(new_width, new_height, MAT_TYPE::U8_C4);
    // Create an OpenCV Mat that shares sl::Mat data
    cv::Mat image_ocv = slMat2cvMat(image_zed);
    // Create HSV image
    cv::Mat image_hsv = image_ocv;
    cv::Mat image_mask = image_ocv;

    Mat depth_image_zed(new_width, new_height, MAT_TYPE::U8_C4);
    cv::Mat depth_image_ocv = slMat2cvMat(depth_image_zed);

    print("Starting Main Loop...\n");

    // Main Loop
    while (prcp_run)
    {
        if(ctrl_telem.mode == FAULT)
        {
            ctrl_cmd.mode = CLEARFAULT;
        }
        else
        {
            ctrl_cmd.mode = RUN;
        }

        // Process Vision Data
        if (zed.grab() == ERROR_CODE::SUCCESS)
        {
            // Retrieve the left image in sl::Mat
            // The cv::Mat is automatically updated
            zed.retrieveImage(image_zed, VIEW::LEFT, MEM::CPU, new_image_size);
            zed.retrieveImage(depth_image_zed, VIEW::DEPTH, MEM::CPU, new_image_size);

            // Lane following
            // --------------
            // Convert image to hsv
            cv::cvtColor(image_ocv, image_hsv, cv::COLOR_BGR2HSV);
            // Detect the object based on HSV Range Values
            inRange(image_hsv, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), image_mask);

            // Display the left image from the cv::Mat object
            cv::imshow("Image", image_ocv);
            cv::imshow("Depth", depth_image_ocv);
            cv::imshow("Prcp", image_mask);

            cv::waitKey(10);
        }
        else
        {
            sleep_ms(1);
            print("Failed to grab frame!\n");
        }

        // Send/Receive the latest telemetry/command data from shared memory to local memory
        comm_prcp_transaction(&ctrl_cmd, &ctrl_telem);

        // Update ctrl_cmd struct
        ctrl_cmd.mode = RUN;
        ctrl_cmd.heartbeat++;
        ctrl_cmd.drive_vel = 0.05;
        cout << "HB = " << ctrl_cmd.heartbeat << endl;
    }

    print("Shuting down prcp...\n");

    // Close OpenCV windows
    cv::destroyAllWindows();

    // close the ZED
    zed.close();

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
    @brief  Signal interrupt to kill real-time control thread
*/
/**************************************************************************/
void sigint(int) 
{
    prcp_run = false;
}




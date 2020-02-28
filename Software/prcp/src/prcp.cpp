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

const uint32_t img_height = 376;
const uint32_t img_width = 672;

const uint32_t window_left = 0;
const uint32_t window_right = 672;
const uint32_t window_center = (window_right - window_left)/2;

const uint32_t window_top = 220;
const uint32_t window_bottom = 320;

// Local Communication Variables
Ctrl_Cmd ctrl_cmd;                              // Command data from C&DH
Ctrl_Telem ctrl_telem;                          // Telemetry data to C&DH

int main(int argc, char **argv) 
{
    // CTRL Command Variables
    ctrl_cmd.mode = Ctrl_Mode.CLEARFAULT;
    ctrl_cmd.heartbeat = 0;
    ctrl_cmd.steer_pos = 0.0;
    ctrl_cmd.drive_vel = 0.0;

    // CTRL calc variabls
    float cmd_steer_max = 1.5

    print("Starting Perception System...\n");

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

    auto camera_infos = zed.getCameraInformation();

    // Create an RGBA sl::Mat object
    sl::Mat image_zed(zed.getResolution(), MAT_TYPE::U8_C4);
    // Create an OpenCV Mat that shares sl::Mat data
    cv::Mat image_ocv = slMat2cvMat(image_zed);

    print("Starting Main Loop...\n");

    // Main Loop
    while (prcp_run)
    {
        // Process Vision Data
        if (zed.grab() == ERROR_CODE::SUCCESS) 
        {
            // Retrieve the left image in sl::Mat
            // The cv::Mat is automatically updated
            zed.retrieveImage(image_zed, VIEW::LEFT);
            // Display the left image from the cv::Mat object
            cv::imshow("Image", image_ocv);
            waitKey(1);
        } 
        else 
        {
            sleep_ms(1);
        }

        // Send/Receive the latest telemetry/command data from shared memory to local memory
        comm_transaction(&ctrl_cmd, &ctrl_telem);
    }
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
    @brief  Signal interrupt to kill real-time control thread
*/
/**************************************************************************/
void sigint(int) 
{
    prcp_run = false;
}




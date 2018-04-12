#include "DC1394Wrapper.h"


DC1394Wrapper::DC1394Wrapper() {
/*  
#ifdef CAM_IFACE_CALL_UNLISTEN
#include <dc1394/kernel-video1394.h> // for VIDEO1394_IOC_UNLISTEN_CHANNEL
#endif
*/

  m_pcFrame = NULL;
  m_bTransmissionStarted = false;
}

DC1394Wrapper::~DC1394Wrapper() {
  Cleanup(m_pcCamera);
}

void DC1394Wrapper::Cleanup(dc1394camera_t *camera) {
  if (m_bTransmissionStarted) {
    StopTransmission();
  }

  dc1394_capture_stop(m_pcCamera);
  dc1394_camera_free(m_pcCamera);
}

int DC1394Wrapper::Init() {

  std::cout << "Instantiating DC1394... ";
  m_pcD = dc1394_new();
  if (! m_pcD)
    return 1;
  std::cout << "Done." << std::endl;

  std::cout << "Listing cameras... ";
  m_eErr = dc1394_camera_enumerate (m_pcD, & m_pcList);
  CheckError(0);
  std::cout << "Done." << std::endl;

  if (m_pcList->num == 0) {
    std::cout << "No cameras found!" << std::endl;
  }

  std::cout << "Instantiating camera... ";
  m_pcCamera = dc1394_camera_new (m_pcD, m_pcList->ids[0].guid);
  if (! m_pcCamera) {
    fprintf(stderr, "Failed to initialize camera with guid %llx \n", m_pcList->ids[0].guid);
    exit(1);
  }
  dc1394_camera_free_list (m_pcList);

  fprintf(stderr, "Initialized camera with guid %llx. \n", m_pcCamera->guid);
  std::cout << "Done." << std::endl;

  // 
  // std::cout << "Powering down camera... ";
  // m_eErr =  dc1394_camera_set_power(m_pcCamera, DC1394_OFF);
  // CheckError(11);
  // std::cout << "Done." << std::endl;

  // std::cout << "Powering up camera... ";
  // m_eErr =  dc1394_camera_set_power(m_pcCamera, DC1394_ON);
  // CheckError(12);
  // std::cout << "Done." << std::endl;

  // std::cout << "Resetting camera... ";
  // m_eErr =  dc1394_reset_camera(m_pcCamera);
  // CheckError(10);
  // std::cout << "Done." << std::endl;

  std::cout << "Setting ISO speed... ";
  m_eErr=dc1394_video_set_iso_speed(m_pcCamera, DC1394_ISO_SPEED_400);
  CheckError(2);
  std::cout << "Done." << std::endl;

  std::cout << "Setting video mode... ";
  m_eErr=dc1394_video_set_mode(m_pcCamera, DC1394_VIDEO_MODE_640x480_MONO16);
  CheckError(3);
  std::cout << "Done." << std::endl;

  std::cout << "Setting frame rate... ";
  m_eErr=dc1394_video_set_framerate(m_pcCamera, DC1394_FRAMERATE_30);
  CheckError(4);
  std::cout << "Done." << std::endl;

  std::cout << "Setting capture flags... ";
  m_eErr=dc1394_capture_setup(m_pcCamera,4, DC1394_CAPTURE_FLAGS_DEFAULT);
  CheckError(5);
  std::cout << "Done." << std::endl;
}

void DC1394Wrapper::Grab() {

  if (! m_bTransmissionStarted) {
    StartTransmission();
  }

  std::cout << "Capturing... ";
  m_eErr=dc1394_capture_dequeue(m_pcCamera, DC1394_CAPTURE_POLICY_WAIT, &m_pcFrame);
  CheckError(6);
  std::cout << "Done." << std::endl;
}

unsigned char * DC1394Wrapper::GetImage() {
  return m_pcFrame->image;
}

void DC1394Wrapper::StartTransmission() {
  std::cout << "Starting video transmission... ";
  m_eErr=dc1394_video_set_transmission(m_pcCamera, DC1394_ON);
  CheckError(7);
  std::cout << "Done." << std::endl;

  m_bTransmissionStarted = true;
}

void DC1394Wrapper::StopTransmission() {
  std::cout << "Stopping video transmission... ";
  m_eErr=dc1394_video_set_transmission(m_pcCamera, DC1394_OFF);
  CheckError(8);
  std::cout << "Done." << std::endl;
}

void DC1394Wrapper::Close() {
  Cleanup(m_pcCamera);
}

void DC1394Wrapper::CheckError(int nStep) {
  if (m_eErr) {
    std::cout << "Something went wrong on step " << nStep << std::endl;
    Cleanup(m_pcCamera);
    exit(1);
  }
}

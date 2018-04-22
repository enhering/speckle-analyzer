#include "DC1394Wrapper.h"


DC1394Wrapper::DC1394Wrapper() {
/*  
#ifdef CAM_IFACE_CALL_UNLISTEN
#include <dc1394/kernel-video1394.h> // for VIDEO1394_IOC_UNLISTEN_CHANNEL
#endif
*/

  m_pcFrame = NULL;
  m_bTransmissionStarted = false;
  m_bRGBBufferAllocated = false;
  m_bCameraInitialized = false;
}

DC1394Wrapper::~DC1394Wrapper() {
  if (m_bCameraInitialized) {
    Cleanup(m_pcCamera);
  }
}

int DC1394Wrapper::Init() {

  // std::cout << "Cleaning up ISO channels and bandwidth... ";
  // dc1394_cleanup_iso_channels_and_bandwidth();
  // std::cout << "Done." << std::endl;

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


  // std::cout << "Resetting bus... ";
  // m_eErr =  dc1394_reset_bus(m_pcCamera);
  // CheckError(20);
  // std::cout << "Done." << std::endl;

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
  m_eErr=dc1394_video_set_framerate(m_pcCamera, DC1394_FRAMERATE_15);
  CheckError(4);
  std::cout << "Done." << std::endl;

  std::cout << "Setting trigger... ";
  // m_eErr = dc1394_external_trigger_set_mode(m_pcCamera, DC1394_TRIGGER_MODE_3);
  m_eErr = dc1394_external_trigger_set_power(m_pcCamera, DC1394_ON);
  // CheckError(22);
  m_eErr = dc1394_external_trigger_set_source(m_pcCamera, DC1394_TRIGGER_SOURCE_SOFTWARE);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_TRIGGER, DC1394_FALSE);
  // CheckError(23);
  std::cout << "Done." << std::endl;


  std::cout << "Setting features: ";
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_BRIGHTNESS, 428);
  std::cout << "-";
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_EXPOSURE, 256);
  std::cout << "/";
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_SHARPNESS, 0);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_WHITE_BALANCE, 0);
  std::cout << "|";
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_HUE, 1792);
  std::cout << "\\";
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_SATURATION, 64);
  std::cout << "-";
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_GAMMA, 1);
  std::cout << "/";
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_SHUTTER, 200);
  std::cout << "|";
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_GAIN, 0);
  std::cout << "\\";
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_IRIS, 0);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_FOCUS, 0);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_TEMPERATURE, 0);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_TRIGGER, 0);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_TRIGGER_DELAY, 0);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_WHITE_SHADING, 0);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_FRAME_RATE, 0);
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_ZOOM, 0);
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_PAN, 0);
  std::cout << "-";
  m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_TILT, 0);
  std::cout << "/";
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_OPTICAL_FILTER, 0);
  std::cout << "|";
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_CAPTURE_SIZE, 0);
  std::cout << "\\";
  // m_eErr = dc1394_feature_set_value(m_pcCamera, DC1394_FEATURE_CAPTURE_QUALITY, 0);
  std::cout << "_ ";
  std::cout << "Done." << std::endl;

  std::cout << "Setting capture flags... ";
  m_eErr=dc1394_capture_setup(m_pcCamera,4, DC1394_CAPTURE_FLAGS_DEFAULT);
  CheckError(5);
  std::cout << "Done." << std::endl;

  std::cout << "Starting video transmission... ";
  m_eErr=dc1394_video_set_transmission(m_pcCamera, DC1394_ON);
  CheckError(7);
  std::cout << "Done." << std::endl;
  m_bTransmissionStarted = true;
  m_bCameraInitialized = true;
}

void DC1394Wrapper::Grab() {

  if (! m_bCameraInitialized) {
    Init();
  }

  bool bCaptureOK = false;

  std::cout << "Pullling trigger... ";
  m_eErr = dc1394_software_trigger_set_power(m_pcCamera, DC1394_ON);
  CheckError(24);
  std::cout << "Ok" << std::endl;

  std::cout << "Capturing... " << std::endl;
  while (! bCaptureOK) {
    m_eErr=dc1394_capture_dequeue(m_pcCamera, DC1394_CAPTURE_POLICY_WAIT, &m_pcFrame);
    CheckError(6);
    if (m_pcFrame == NULL) {
      std::cout << "Empty buffer... Waiting 1msec. " << std::endl;  
      usleep(1000);
    }
    else {
      bCaptureOK = true;
      break;
    }
  }
  std::cout << "Done." << std::endl;
}

void DC1394Wrapper::AllocateRGBBuffer() {
  FreeRGBBuffer();  
  uint32_t nBufferSize = m_pcFrame->image_bytes * 3;
  m_panRGBBuffer = (unsigned char *) malloc(nBufferSize);
  m_bRGBBufferAllocated = true;  
}

unsigned char * DC1394Wrapper::GetRawImage() {
  return m_pcFrame->image;
}

void DC1394Wrapper::ReleaseFrame() {
  // Only necessary after GetRawImage
  m_eErr = dc1394_capture_enqueue(m_pcCamera, m_pcFrame);
  CheckError(44);
}

unsigned char * DC1394Wrapper::GetRGBImage() {
  AllocateRGBBuffer();

  uint32_t nBufferSize = m_pcFrame->image_bytes * 3;

  m_eErr = dc1394_bayer_decoding_16bit((uint16_t * ) m_pcFrame->image, 
                                       (uint16_t * ) m_panRGBBuffer,
                                       m_pcFrame->size[0],
                                       m_pcFrame->size[1],
                                       DC1394_COLOR_FILTER_RGGB,
                                       DC1394_BAYER_METHOD_BILINEAR,
                                       16);
  CheckError(43);
  m_eErr = dc1394_capture_enqueue(m_pcCamera, m_pcFrame);
  CheckError(44);

  return m_panRGBBuffer;
}

void DC1394Wrapper::FreeRGBBuffer() {
  if (m_bRGBBufferAllocated) {
    free(m_panRGBBuffer);
  }
  m_bRGBBufferAllocated = false;
}

void DC1394Wrapper::StopTransmission() {

  if (m_bTransmissionStarted) {
    std::cout << "Stopping video transmission... ";
    m_eErr=dc1394_video_set_transmission(m_pcCamera, DC1394_OFF);
    CheckError(8);
    std::cout << "Done." << std::endl;
  }
}

void DC1394Wrapper::Cleanup(dc1394camera_t *camera) {
  FreeRGBBuffer();
  StopTransmission();

  dc1394_capture_stop(m_pcCamera);
  dc1394_camera_free(m_pcCamera);
  dc1394_free(m_pcD);

  m_bCameraInitialized = false;
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

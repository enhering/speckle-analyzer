#include "DC1394Wrapper.h"


DC1394Wrapper::DC1394Wrapper() {
/*  
#ifdef CAM_IFACE_CALL_UNLISTEN
#include <dc1394/kernel-video1394.h> // for VIDEO1394_IOC_UNLISTEN_CHANNEL
#endif
*/

  m_pcFrame = NULL;
}

DC1394Wrapper::~DC1394Wrapper() {
  Cleanup(m_pcCamera);
}

void DC1394Wrapper::Cleanup(dc1394camera_t *camera) {
  dc1394_video_set_transmission(m_pcCamera, DC1394_OFF);
  dc1394_capture_stop(m_pcCamera);
  dc1394_camera_free(m_pcCamera);
}

int DC1394Wrapper::Init() {
  std::cout << "Initializing camera..." ;

  m_pcD = dc1394_new ();

  if (! m_pcD)
    return 1;
    m_eErr = dc1394_camera_enumerate (m_pcD, & m_pcList);
    DC1394_ERR_RTN(m_eErr,"Failed to enumerate cameras");

  if (m_pcList->num == 0) {
    fprintf(stderr, "No cameras found");
    return 1;
  }

  m_pcCamera = dc1394_camera_new (m_pcD, m_pcList->ids[0].guid);
  if (! m_pcCamera) {
    fprintf(stderr, "Failed to initialize camera with guid %llx", m_pcList->ids[0].guid);
    return 1;
  }
  dc1394_camera_free_list (m_pcList);

  //printf("Using camera with GUID %"PRIx64"\n", m_pcCamera->guid);
  std::cout << "Using camera with GUID " << m_pcCamera->guid << ". ";

  /*-----------------------------------------------------------------------
   *  setup capture
   *-----------------------------------------------------------------------*/

  m_eErr=dc1394_reset_bus(m_pcCamera);
  if (m_eErr) {
    std::cout << "Could not reset the bus" << std::endl;
    Cleanup(m_pcCamera);
    exit(1);
  }

  m_eErr=dc1394_reset_camera(m_pcCamera);
  if (m_eErr) {
    std::cout << "Could not reset the camera" << std::endl;
    Cleanup(m_pcCamera);
    exit(1);
  }  

  m_eErr=dc1394_video_set_iso_speed(m_pcCamera, DC1394_ISO_SPEED_400);
  if (m_eErr) {
    std::cout << "Could not set iso speed" << std::endl; 
    Cleanup(m_pcCamera);
    exit(1);
  }

  m_eErr=dc1394_video_set_mode(m_pcCamera, DC1394_VIDEO_MODE_640x480_MONO8);
  if (m_eErr) {
    std::cout << "Could not set video mode" << std::endl;
    Cleanup(m_pcCamera);
    exit(1);
  }

  m_eErr=dc1394_video_set_framerate(m_pcCamera, DC1394_FRAMERATE_15);
  if (m_eErr) {
    std::cout << "Could not set framerate"  << std::endl;
    Cleanup(m_pcCamera);
    exit(1);
  }

  m_eErr=dc1394_capture_setup(m_pcCamera,4, DC1394_CAPTURE_FLAGS_DEFAULT);
  if (m_eErr) {
    std::cout << "Could not setup camera-\nmake sure that the video mode and framerate are\nsupported by your camera" << std::endl;
    Cleanup(m_pcCamera);
    exit(1);
  }
  std::cout << "Done." << std::endl;
}

void DC1394Wrapper::Grab() {

  std::cout << "Capturing... " << std::endl;

  m_eErr=dc1394_video_set_transmission(m_pcCamera, DC1394_ON);
  if (m_eErr) {
    std::cout << "Could not start camera iso transmission" << std::endl;
    Cleanup(m_pcCamera);
    exit(1);
  }

  m_eErr=dc1394_capture_dequeue(m_pcCamera, DC1394_CAPTURE_POLICY_WAIT, &m_pcFrame);
  if (m_eErr) {
    std::cout << "Could not capture a frame" << std::endl;
    Cleanup(m_pcCamera);
    exit(1);  
  }

  m_eErr=dc1394_video_set_transmission(m_pcCamera,DC1394_OFF);
  if (m_eErr) {
    std::cout << "Could not stop the camera." << std::endl;
    Cleanup(m_pcCamera);
    exit(1);  
  }

  std::cout << " Done." << std::endl;
  //dc1394_get_image_size_from_video_mode(m_pcCamera, DC1394_VIDEO_MODE_640x480_RGB8, &m_nWidth, &m_nHeight);

  /*-----------------------------------------------------------------------
   *  save image as 'Image.pgm'
   *-----------------------------------------------------------------------*/
  // m_pImageFile = fopen(IMAGE_FILE_NAME, "wb");

  // if( m_pImageFile == NULL) {
  //   perror( "Can't create output file");
  //   CleanupAndExit(m_pcCamera);
  // }

  // dc1394_get_image_size_from_video_mode(m_pcCamera, DC1394_VIDEO_MODE_640x480_RGB8, &m_nWidth, &m_nHeight);

  // fprintf(m_pImageFile,"P6\n%u %u\n255\n", m_nWidth, m_nHeight);
  // fwrite(frame->image, 1, m_nHeight*m_nWidth*3, m_pImageFile);
  // fclose(m_pImageFile);
  
  // printf("wrote: " IMAGE_FILE_NAME " (%d image bytes)\n",m_nHeight*m_nWidth*3);

}

void DC1394Wrapper::GetImage(unsigned char * pachBuffer) {
  pachBuffer = (unsigned char *) malloc( m_pcFrame->allocated_image_bytes );
  if (pachBuffer == NULL) {
    fprintf(stderr,"couldn't allocate memory in %s, line %d\n",__FILE__,__LINE__);
    exit(1);
  }

  memcpy(pachBuffer, m_pcFrame->image, sizeof(pachBuffer));
}

void DC1394Wrapper::Close() {
  Cleanup(m_pcCamera);
}
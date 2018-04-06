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
  dc1394_video_set_transmission(m_pCamera, DC1394_OFF);
  dc1394_capture_stop(m_pCamera);
  dc1394_camera_free(m_pCamera);
}

void DC1394Wrapper::CleanupAndExit(dc1394camera_t *camera) {
  dc1394_video_set_transmission(m_pCamera, DC1394_OFF);
  dc1394_capture_stop(m_pCamera);
  dc1394_camera_free(m_pCamera);
  exit(1);
}

int DC1394Wrapper::Init() {
  m_cD = dc1394_new ();

  if (! m_cD)
    return 1;
    m_cErr = dc1394_camera_enumerate (d, & m_pcList);
    DC1394_ERR_RTN(m_cErr,"Failed to enumerate cameras");

  if (m_cList->num == 0) {
    fprintf(stdd::cerr, "No cameras found");
    return 1;
  }

  m_pcCamera = dc1394_camera_new (d, m_cList->ids[0].guid);
  if (! m_pcCamera) {
    fprintf(stdd::cerr, "Failed to initialize camera with guid %llx", list->ids[0].guid);
    return 1;
  }
  dc1394_camera_free_list (list);

  printf("Using camera with GUID %"PRIx64"\n", camera->guid);

  /*-----------------------------------------------------------------------
   *  setup capture
   *-----------------------------------------------------------------------*/

  m_cErr=dc1394_video_set_iso_speed(m_pcCamera, DC1394_ISO_SPEED_400);
  DC1394_ERR_CLN_RTN(m_cErr,CleanupAndExit(m_pcCcamera),"Could not set iso speed");

  m_cErr=dc1394_video_set_mode(m_pcCamera, DC1394_VIDEO_MODE_640x480_RGB8);
  DC1394_ERR_CLN_RTN(m_cErr,CleanupAndExit(m_pcCcamera),"Could not set video mode\n");

  m_cErr=dc1394_video_set_framerate(m_pcCamera, DC1394_FRAMERATE_7_5);
  DC1394_ERR_CLN_RTN(m_cErr,CleanupAndExit(m_pcCcamera),"Could not set framerate\n");

  m_cErr=dc1394_capture_setup(m_pcCamera,4, DC1394_CAPTURE_FLAGS_DEFAULT);
  DC1394_ERR_CLN_RTN(m_cErr,CleanupAndExit(m_pcCcamera),"Could not setup camera-\nmake sure that the video mode and framerate are\nsupported by your camera\n");
}

void DC1394Wrapper::Grab() {
  /*-----------------------------------------------------------------------
   *  have the camera start sending us data
   *-----------------------------------------------------------------------*/
  m_cErr=dc1394_video_set_transmission(m_pcCamera, DC1394_ON);
  DC1394_ERR_CLN_RTN(m_cErr,cleanup_and_exit(camera),"Could not start camera iso transmission\n");

  /*-----------------------------------------------------------------------
   *  capture one frame
   *-----------------------------------------------------------------------*/
  m_cErr=dc1394_capture_dequeue(m_pcCamera, DC1394_CAPTURE_POLICY_WAIT, &frame);
  DC1394_ERR_CLN_RTN(m_cErr,cleanup_and_exit(camera),"Could not capture a frame\n");

  /*-----------------------------------------------------------------------
   *  stop data transmission
   *-----------------------------------------------------------------------*/
  m_cErr=dc1394_video_set_transmission(m_pcCamera,DC1394_OFF);
  DC1394_ERR_CLN_RTN(m_cErr,cleanup_and_exit(camera),"Could not stop the camera?\n");

  /*-----------------------------------------------------------------------
   *  save image as 'Image.pgm'
   *-----------------------------------------------------------------------*/
  m_pImagefile = fopen(IMAGE_FILE_NAME, "wb");

  if( m_pImagefile == NULL) {
    perror( "Can't create output file");
    CleanupAndExit(m_pcCamera);
  }

  dc1394_get_image_size_from_video_mode(m_pcCamera, DC1394_VIDEO_MODE_640x480_RGB8, &m_nWidth, &m_nHeight);

  fprintf(m_pImagefile,"P6\n%u %u\n255\n", m_nWidth, m_nHeight);
  fwrite(frame->image, 1, m_nHeight*m_nWidth*3, m_pImagefile);
  fclose(m_pImagefile);
  
  printf("wrote: " IMAGE_FILE_NAME " (%d image bytes)\n",m_nHeight*m_nWidth*3);

}

void DC1394Wrapper::Close() {
 /*-----------------------------------------------------------------------
   *  close camera
   *-----------------------------------------------------------------------*/
  dc1394_video_set_transmission(m_pcCamera, DC1394_OFF);
  dc1394_capture_stop(m_pcCamera);
  dc1394_camera_free(m_pcCamera);
  dc1394_free (d);
}
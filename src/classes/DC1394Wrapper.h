#ifndef DC1394WRAPPER_H_
  #define #DC1394WRAPPER_H_

  #include "Base.h"
  #include <dc1394/dc1394.h>
  #include <dc1394/conversions.h>
  #include <dc1394/control.h>
  #include <dc1394/utils.h>

  class DC1394Wrapper.h : public Base {
    public:
      DC1394Wrapper();
      ~DC1394Wrapper();

      static void CleanupAndExit(dc1394camera_t *);

      void Init();

      void Grab();

      void Close();

    private:
      FILE                * m_pImageFile;
      dc1394camera_t      * m_pcCamera;
      unsigned int          m_nWidth, m_nHeight;
      dc1394video_frame_t * m_pcFrame;
      dc1394featureset_t    m_cFeatures;
      dc1394_t            * m_pcD;
      dc1394camera_list_t * m_pcList;
      dc1394error_t         m_cErr;

  }
#endif
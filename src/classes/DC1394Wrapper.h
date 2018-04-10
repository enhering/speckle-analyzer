#ifndef DC1394WRAPPER_H_
  #define DC1394WRAPPER_H_

  #include "Base.h"

  class DC1394Wrapper : public Base {
    public:
      DC1394Wrapper();
      ~DC1394Wrapper();

      void Cleanup(dc1394camera_t *);

      int Init();

      void Grab();

      unsigned char * GetImage()       { return m_pcFrame->image;   }
      uint32_t        GetImageWidth()  { return m_pcFrame->size[0]; }
      uint32_t        GetImageHeight() { return m_pcFrame->size[1]; }

      void Close();

    private:
      FILE                * m_pImageFile;
      dc1394camera_t      * m_pcCamera;
      unsigned int          m_nWidth, m_nHeight;
      dc1394video_frame_t * m_pcFrame;
      dc1394featureset_t    m_cFeatures;
      dc1394_t            * m_pcD;
      dc1394camera_list_t * m_pcList;
      dc1394error_t         m_eErr;

  };

  // static dc1394camera_t      * m_pcCamera;
#endif
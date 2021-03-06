#ifndef DC1394WRAPPER_H_
  #define DC1394WRAPPER_H_

  #include "opencv2/opencv.hpp"

  #include "Base.h"

  class DC1394Wrapper : public Base {
    public:
      DC1394Wrapper();
      ~DC1394Wrapper();

      int Init();

      void Grab();

      void AllocateRGBBuffer();
      unsigned char * GetRGBImage();
      void ReleaseFrame();
      unsigned char * GetRawImage();
      uint32_t GetImageWidth()  { return m_nWidth; }
      uint32_t GetImageHeight() { return m_nHeight; }
      uint32_t GetImageSize()   { return m_nSize; }

      void  CheckError(int);

      void StopTransmission();
      void FreeRGBBuffer();
      void Cleanup(dc1394camera_t *);

      cv::Mat CaptureImage();


      void Close();

    private:
      FILE                * m_pImageFile;
      dc1394camera_t      * m_pcCamera;
      unsigned int          m_nWidth, m_nHeight, m_nSize;
      dc1394video_frame_t * m_pcFrame;
      dc1394featureset_t    m_cFeatures;
      dc1394_t            * m_pcD;
      dc1394camera_list_t * m_pcList;
      dc1394error_t         m_eErr;

      unsigned char       * m_panRGBBuffer;
      bool                  m_bRGBBufferAllocated;

      bool                  m_bTransmissionStarted;
      bool                  m_bCameraInitialized;

  };

  // static dc1394camera_t      * m_pcCamera;
#endif
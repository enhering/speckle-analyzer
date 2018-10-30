#ifndef SAMPLE_H_
  #define SAMPLE_H_

  #include "Base.h"

  class Sample : public Base {
    public:
      Sample();
      ~Sample();
    private:
      std::string m_strSampleName;

  };

#endif

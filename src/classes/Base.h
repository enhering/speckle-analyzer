#ifndef BASE_H_
  #define BASE_H_

  #include <stdlib.h>
  #include <stdio.h>
  #include <iostream>
  #include <fstream>
  #include <cstring>
  #include <unistd.h>#include "opencv2/opencv.hpp"


  #include <dc1394/dc1394.h>
  #include <dc1394/conversions.h>
  #include <dc1394/control.h>
  #include <dc1394/utils.h>

  #include "opencv2/opencv.hpp"

  class Base {
    public:
      Base();
      ~Base();
  };

#endif
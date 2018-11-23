#include <iostream>

#include "TApplication.h"
#include "TSystem.h"

#include "DC1394Wrapper.h"
#include "Speckle.h"

int g_nMouseX, g_nMouseY;

DC1394Wrapper * g_pDC1394Wrapper;
Speckle * g_pSpeckle;
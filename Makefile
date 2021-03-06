HGPUSHURL=ssh://hg@bitbucket.org/enhering/speckle-analyzer
HGURL=https://enhering@bitbucket.org/enhering/speckle-analyzer

BINDIR=bin/
OBJDIR=bin/

CLASSES_SRC_DIR=src/classes
EXEC_SRC_DIR=src/

ifneq ($(wildcard FREEBSD),)
  OS=FREEBSD
endif
ifneq ($(wildcard MACOS),)
  OS=MACOS
endif
ifneq ($(wildcard RASPBIAN),)
  OS=RASPBIAN
endif
ifneq ($(wildcard DEBIAN),)
  OS=DEBIAN
endif

ifeq ($(OS),MACOS)
  COMPILER=llvm-g++ -w

  ICGICC=-I/usr/include
  LCGICC=-L/usr/lib -L/Users/enhering/tmp/cgicc-3.2.12/cgicc/.libs -lcgicc

  IOPENCV=-I/usr/local/include/opencv -I/usr/local/include
  LOPENCV=-L/usr/local/Cellar/opencv/3.4.0_1/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_photo -lopencv_fuzzy -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ml -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc -lopencv_core

  IBOOST=-I/usr/local/Cellar/boost/1.64.0_1/include/
  LBOOST=-L/usr/local/Cellar/boost/1.64.0_1/lib/ -lboost_regex-mt

  IROOT=-stdlib=libc++ -std=c++11 -m64 -I/Applications/root_v6.12.06/include
  LROOT=-Wl,-rpath,/Applications/root_v6.12.06/lib -L/Applications/root_v6.12.06/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -lpthread -stdlib=libc++ -lm -ldl

  IMYSQLCPPCONN=-I/opt/local/include/
  LMYSQLCPPCONN=-L/opt/local/lib -lmysqlcppconn

  ICRYPTOPP=-I/opt/local/include/
  LCRYPTOPP=-L/opt/local/lib -lcryptopp

  DOCUMENT_ROOT=~/Sites/mms
  CGI_BIN=~/Sites/

  IJSON=-I/Users/enhering/tmp/libjson
  LJSON=-L/Users/enhering/tmp/libjson -ljson

  IXML=-I/usr/local/Cellar/libxml2/2.9.3/include/libxml2/
  LXML=-L/usr/local/Cellar/libxml2/2.9.3/lib/libxml2.a -lxml2

  LNCURSES=-lncurses
endif

ifeq ($(OS),FREEBSD)
  COMPILER=clang++ -w
  ICGICC=-I/usr/local/include/
  LCGICC=-L/usr/local/lib -lcgicc

  IOPENCV=-I/usr/local/include/opencv -I/usr/local/include
  LOPENCV=-L/usr/local/lib -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -lpthread -lm

  IBOOST=-I/usr/local/include
  LBOOST=-L/usr/local/lib -lboost_regex-mt

  IMYSQLCPPCONN=-I/usr/local/include/
  LMYSQLCPPCONN=-L/usr/local/lib -l mysqlcppconn

	ICRYPTOPP=-I/opt/local/include/
  LCRYPTOPP=-L/opt/local/lib -lcryptopp

  IXML=-I/usr/local/include/libxml2
  LXML=-L/usr/local/lib -lxml2

  DOCUMENT_ROOT=/usr/local/www/apache24/data/
  CGI_BIN=/usr/local/www/apache24/data/

  IJSON=-I~/tmp/libjson
  LJSON=-L~/tmp/libjson -ljson

  LNCURSES=-lncurses
endif

ifeq ($(OS),DEBIAN)
  COMPILER=g++ -w
  ICGICC=-I/usr/local/include/
  LCGICC=-L/usr/local/lib -lcgicc

  IOPENCV=-I/usr/local/include/opencv -I/usr/local/include
  LOPENCV=-L/usr/local/lib -lopencv_dnn -lopencv_ml -lopencv_objdetect -lopencv_shape -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core

  IROOT=-pthread -std=c++11 -m32 -msse -mfpmath=sse -I/usr/local/root/include
  LROOT=-L/usr/local/root/lib -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic

  IBOOST=-I/usr/local/include
  LBOOST=-L/usr/local/lib -lboost_regex-mt

  IMYSQLCPPCONN=-I/usr/local/include/
  LMYSQLCPPCONN=-L/usr/local/lib -l mysqlcppconn

	ICRYPTOPP=-I/opt/local/include/
  LCRYPTOPP=-L/opt/local/lib -lcryptopp

  IXML=-I/usr/local/include/libxml2
  LXML=-L/usr/local/lib -lxml2

  DOCUMENT_ROOT=/usr/local/www/apache24/data/
  CGI_BIN=/usr/local/www/apache24/data/

  IJSON=-I~/tmp/libjson
  LJSON=-L~/tmp/libjson -ljson

  LNCURSES=-lncurses
endif

BINDIR=bin
OBJDIR=bin

SOURCE_DIR = src
CLASSES_DIR = $(SOURCE_DIR)/classes

SRC_FILES=$(CLASSES_SRC_DIR)/DC1394Wrapper.cpp    \
          $(CLASSES_SRC_DIR)/Base.cpp             \
          $(CLASSES_SRC_DIR)/Speckle.cpp          \


CXX = $(COMPILER)
CC = $(CXX)

DEBUG_LEVEL     = -g
EXTRA_CCFLAGS   = -Wall -w --std=c++11

CXXFLAGS        = $(DEBUG_LEVEL) $(EXTRA_CCFLAGS)
CCFLAGS         = $(CXXFLAGS)

CPPFLAGS        = -I$(SOURCE_DIR) -I$(CLASSES_DIR) $(IXML) $(IROOT) $(LOPENCV)  $(LROOT) -ldc1394

LDFLAGS         = -L$(SOURCE_DIR)

LDLIBS          =

O_FILES         = $(SRC_FILES:%.cpp=%.o)


COMPILE = $(CXX) $(CXXFLAGS) $(CPPFLAGS)
OUTPUT_OPTION = -o $@


all: $(BINDIR)/speckle-analyzer

$(BINDIR)/speckle-analyzer: $(EXEC_SRC_DIR)/speckle-analyzer.cpp $(O_FILES)
	@/bin/echo -n Compiling $@...
	@$(COMPILE) $^ -o $@
	@echo Ok.

%.o: %.cpp
	@/bin/echo -n Compiling $@...
	@$(COMPILE) -c $< $(OUTPUT_OPTION)
	@echo Ok.

# $(CLASSES_DIR)/BuildNumber.h: $(shell find src -type f)
# 	@/bin/echo -n Generating new build number...
# 	@cat build.number
# 	@sh make_buildnum.sh
# 	@echo Ok.

depend:
	makedepend -- $(CPPFLAGS) -- -Y $(SRC_FILES)

clean:
	@/bin/echo -n Removing build files...
	@$(RM) $(O_FILES) core *.rpo
	@$(RM) -rf bin/*
	@echo Ok.


# INCLUDES=-Isrc/classes $(IOPENCV) $(IROOT)

# all: $(BINDIR)/speckle-analyzer

# $(OBJDIR)/speckle-analyzer: $(EXEC_SRC_DIR)/speckle-analyzer.cpp \
# 	                          $(EXEC_SRC_DIR)/speckle-analyzer.h   \
# 	                          $(OBJDIR)/DC1394Wrapper

# 	@echo 'speckle-analyzer'
# 	@$(COMPILER) $(EXEC_SRC_DIR)/speckle-analyzer.cpp       \
# 							$(OBJDIR)/Base                             \
# 	            $(OBJDIR)/DC1394Wrapper                    \
#               $(INCLUDES)  $(LOPENCV)  $(LROOT) -ldc1394 \
#               -o $(BINDIR)/speckle-analyzer

# $(OBJDIR)/DC1394Wrapper: $(CLASSES_SRC_DIR)/DC1394Wrapper.cpp \
# 	                       $(CLASSES_SRC_DIR)/DC1394Wrapper.h   \
# 	                       $(OBJDIR)/Base
# 	@echo 'DC1394Wrapper'
# 	@$(COMPILER) -c $(CLASSES_SRC_DIR)/DC1394Wrapper.cpp  \
#               $(INCLUDES) -o $(BINDIR)/DC1394Wrapper

# $(OBJDIR)/Base: $(CLASSES_SRC_DIR)/Base.cpp \
# 	              $(CLASSES_SRC_DIR)/Base.h
# 	@echo 'Base'
# 	@$(COMPILER) -c $(CLASSES_SRC_DIR)/Base.cpp  \
#               $(INCLUDES) -o $(BINDIR)/Base


# $(OBJDIR)/View.o: $(CLASSES_SRC_DIR)/View.cpp $(CLASSES_SRC_DIR)/View.h
# 	@echo 'View.o'
# 	@$(COMPILER) -c $(CLASSES_SRC_DIR)/View.cpp $(INCLUDES) -o $(OBJDIR)/View.o

# clean:
# 	@rm -rf bin/*

push:
	@hg push $(HGPUSHURL)
	@echo "Pushed current version to repo."

pull:
	@hg pull -u $(HGURL)
	@echo "Pulled from repo and updated."
# DO NOT DELETE

src/classes/DC1394Wrapper.o: src/classes/DC1394Wrapper.h src/classes/Base.h
src/classes/Base.o: src/classes/Base.h
src/classes/Speckle.o: src/classes/Speckle.h src/classes/Base.h

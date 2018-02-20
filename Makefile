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

ifeq ($(OS),MACOS)
  COMPILER=llvm-g++ -w 
  ICGICC=-I/usr/include
  LCGICC=-L/usr/lib -L/Users/enhering/tmp/cgicc-3.2.12/cgicc/.libs -lcgicc

  IOPENCV=
  LOPENCV=-L/usr/local/Cellar/opencv/3.4.0_1/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_photo -lopencv_fuzzy -lopencv_img_hash -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_dnn -lopencv_plot -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ml -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_flann -lopencv_xobjdetect -lopencv_imgcodecs -lopencv_objdetect -lopencv_xphoto -lopencv_imgproc -lopencv_core

  IBOOST=-I/usr/local/Cellar/boost/1.64.0_1/include/
  LBOOST=-L/usr/local/Cellar/boost/1.64.0_1/lib/ -lboost_regex-mt

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

  IOPENCV=
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

ifeq ($(OS),RASPBIAN)
  #COMPILER=llvm-g++ -w -std=c++0x
  COMPILER=g++ -w -std=c++0x 
  ICGICC=-I/usr/local/include
  LCGICC=-L/usr/local/lib -lcgicc

  IOPENCV=
  LOPENCV=

	IBOOST=-I/usr/local/include
  LBOOST=-L/usr/lib -lboost_regex-mt

  IGSL=-I/usr/local/include/
  LGSL=-L/usr/local/lib -lgsl

  IMGL=-I/usr/local/include
  LMGL=-L/usr/local/lib -lmgl

  IXML=-I/usr/include/libxml2/
  LXML=-L/usr/lib/arm-linux-gnueabihf/ -lxml2

  DOCUMENT_ROOT=/usr/home/$(USERNAME)/public_html
  CGI_BIN=/usr/home/$(USERNAME)/public_html

  IJSON=-I/home/pi/tmp/libjson
  LJSON=-L/home/pi/tmp/libjson -ljson

  LNCURSES=-lncurses 

  ARM_PLATFORM=1
endif

INCLUDES=-Isrc/classes $(IOPENCV)

all: $(BINDIR)/speckle-analyzer

$(OBJDIR)/speckle-analyzer: $(EXEC_SRC_DIR)/speckle-analyzer.cpp $(EXEC_SRC_DIR)/speckle-analyzer.h 
	@echo 'speckle-analyzer'
	@$(COMPILER) $(EXEC_SRC_DIR)/speckle-analyzer.cpp  \
               $(INCLUDES)  $(LOPENCV)  \
               -o $(BINDIR)/speckle-analyzer

# $(OBJDIR)/View.o: $(CLASSES_SRC_DIR)/View.cpp $(CLASSES_SRC_DIR)/View.h 
# 	@echo 'View.o'
# 	@$(COMPILER) -c $(CLASSES_SRC_DIR)/View.cpp $(INCLUDES) -o $(OBJDIR)/View.o

clean:
	@rm -rf bin/*

push: 
	@hg push $(HGPUSHURL)
	@echo "Pushed current version to repo."

pull: 
	@hg pull -u $(HGURL)
	@echo "Pulled from repo and updated."

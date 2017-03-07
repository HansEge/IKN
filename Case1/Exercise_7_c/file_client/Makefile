####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DLIB_LIBRARY -DQT_NO_DEBUG
CFLAGS        = -m32 -pipe -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
CXXFLAGS      = -m32 -pipe -O2 -Wall -W -D_REENTRANT -fPIC $(DEFINES)
INCPATH       = -I.
LINK          = g++
LFLAGS        = -m32 -Wl,-O1 -shared -Wl,-soname,liblib8.so.1
LIBS          = $(SUBLIBS)  -L/usr/lib/x86_64-linux-gnu -lpthread 
QMAKE         = /usr/lib/x86_64-linux-gnu/qt4/bin/qmake
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = iknlib.c 
OBJECTS       = iknlib.o
HEADER        = iknlib.h

QMAKE_TARGET  = iknlib
DESTDIR       = 
TARGET        = libiknlib.so.1.0.0
TARGETD       = libiknlib.so.1.0.0
TARGET0       = libiknlib.so
TARGET1       = libiknlib.so.1
TARGET2       = libiknlib.so.1.0

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile  $(TARGET)

$(TARGET):  $(OBJECTS) $(SUBLIBS) $(OBJCOMP)  
	-$(DEL_FILE) $(TARGET) $(TARGET0) $(TARGET1) $(TARGET2)
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS) $(OBJCOMP)

clean:
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) iknlib.o
	
####### Compile

iknlib.o: iknlib.c iknlib.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o iknlib.o iknlib.c

####### Install

install_target: first FORCE
	@$(CHK_DIR_EXISTS) $(INSTALL_ROOT)/usr/lib/ || $(MKDIR) $(INSTALL_ROOT)/usr/lib/ 
	-$(INSTALL_PROGRAM) "$(TARGET)" "$(INSTALL_ROOT)/usr/lib/$(TARGET)"
	-$(INSTALL_PROGRAM) "$(HEADER)" "$(INSTALL_ROOT)/usr/include/$(HEADER)"
	-$(STRIP) --strip-unneeded "$(INSTALL_ROOT)/usr/lib/$(TARGET)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)/usr/lib/$(TARGET0)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)/usr/lib/$(TARGET1)"
	-$(SYMLINK) "$(TARGET)" "$(INSTALL_ROOT)/usr/lib/$(TARGET2)"

uninstall_target:  FORCE
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/lib/$(TARGET)" 
	-$(DEL_FILE) "$(INSTALL_ROOT)/usr/include/$(HEADER)" 
	 -$(DEL_FILE) "$(INSTALL_ROOT)/usr/lib/$(TARGET0)" 
	 -$(DEL_FILE) "$(INSTALL_ROOT)/usr/lib/$(TARGET1)" 
	 -$(DEL_FILE) "$(INSTALL_ROOT)/usr/lib/$(TARGET2)"


install:  install_target  FORCE

uninstall: uninstall_target   FORCE

FORCE:

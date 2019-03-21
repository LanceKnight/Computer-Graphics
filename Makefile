SHELL = /bin/sh
prefix = /usr/lib/x86_64-linux-gnu
CC = gcc
C++ = g++
GLUT_LIBS = $(prefix)/libglut.so.3
X_LIBADD =  -lXmu -lXext -lXi -lX11
INCLUDES = -Iinclude -I/usr/include  
LDADD = $(GLUT_LIBS) $(prefix)/libGLU.so.1 $(prefix)/libGL.so  -lm
DEBUG = -DDEBUG
CFLAGS = $(DEBUG) -g -std=c++11 -O2 -Wall -fomit-frame-pointer -ffast-math -fexpensive-optimizations -D_REENTRANT
#CFLAGS = $(DEBUG) -g -std=c++11 -O0 -Wall -D_REENTRANT

COMPILE = $(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
LINK = $(C++) $(CFLAGS) $(LDFLAGS) -o $@

.SUFFIXES:
.SUFFIXES: .cpp .c .o 

.c.o:
	$(COMPILE) -c $<

.cpp.o:
	$(C++) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS) -c $<

all: main

CLI_OBJECTS=main.o Dispatcher.o Util.o Mover.o Drawer.o Colorer.o Reader.o \
			TiffStat.o TiffRead.o TiffWrite.o Resize.o Zoom.o Border.o Select.o\
			3D.o Push.o Pop.o Translate.o Ortho.o Lookat.o Vertex.o Reset.o

main: $(CLI_OBJECTS)
	$(LINK) $(CLI_OBJECTS) $(LDADD) $(LIBS)

clean:
	-rm -f *.o $(PROGRAMS)


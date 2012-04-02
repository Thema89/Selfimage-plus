#
# Author  :  J.H. Meijer
# Date    :  07-11-'03
# Compiler:  g++
#

#
#   Explanation of used symbols (for use with DJGPP)
#
#   ^ == Source file name as stated before ':'
#   @ == Output file name as stated after  ':'
#
#gcc -o conftest -g -O2 -I/usr/include/SDL -D_REENTRANT   conftest.c  -L/usr/lib -Wl,-rpath,/usr/lib -lSDL -lpthread 1>&5

#    Define a macro with name "DEBUG" and value "-g -Wall".

CC   = cc
GCC  = gcc
CPP  = g++
LINK = ld
ASM  = nasm


LIBDIRS		= -L/usr/lib
LIBC		= -( -lsupcxx -lstdcxx -lgcc -lc -)      # The -l option adds lib before the filename and .a after the file name.
LIBCPP		= -lstdc++ -lm
CFLAGS_DYN_LIB	= -fPIC -Wall -g
CFLAGS		= -Wall $(CFLAGS_DYN_LIB) `pkg-config --cflags gtk+-2.0, gthread-2.0`
LIBS		= -lz #/usr/lib/libduma.a #/usr/lib/libefence.a


#declare some vars for dependencies
#
MAKEFILE  = makefile
GLOBAL    = ./src/global.h


all: GTKselfimage


clean:
	rm -f *.exe
	rm -f *.o
	rm -f sample


OBJS = main.o SelfImage_TImageStore.o SelfImage_Utility.o alert.o buttons.o myevents.o combobox.o copydata.o main_ui.o DriveList.o sort.o ReadThread.o WriteThread.o MutexClass.o ThreadClass.o FifoBuffer.o

GTKselfimage:	$(OBJS) $(MAKEFILE)
	$(CPP) $(OBJS) -o $@ $(LIBS) `pkg-config --libs gtk+-2.0, gthread-2.0` 

main.o: ./src/main.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

SelfImage_ReadThread.o: ./src/SelfImage_ReadThread.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

SelfImage_TImageStore.o: ./src/SelfImage_TImageStore.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

SelfImage_Utility.o: ./src/SelfImage_Utility.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

alert.o: ./src/alert/alert.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

buttons.o: ./src/ui/buttons.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

myevents.o: ./src/ui/myevents.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

combobox.o: ./src/ui/combobox.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

copydata.o: ./src/CopyData.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

main_ui.o: ./src/ui/main_ui.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

DriveList.o: ./src/DriveList.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@
	
sort.o: ./src/sort.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

WriteThread.o: ./src/WriteThread.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

ReadThread.o: ./src/ReadThread.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

ThreadClass.o: ./src/ThreadClass.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

MutexClass.o: ./src/MutexClass.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@

FifoBuffer.o: ./src/FifoBuffer.cpp $(MAKEFILE)
	$(CPP) $(CFLAGS) -c $< -o $@



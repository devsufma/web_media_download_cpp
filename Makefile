
CC = g++
DEBUG = -g
CFLAGS = -Wall $(DEBUG)
LFLAGS = -Wall $(DEBUG)

SRCS = src/main.cpp src/lista.cpp src/downloader.cpp src/window.cpp

HDRS = src/lista.h src/downloader.h src/window.h

OBJS = src/main.o src/lista.o src/downloader.o src/window.o
EXE = app

LIBS = -lncurses

$(EXE): $(OBJS) $(HDRS) Makefile
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f downloads/CSS/* downloads/HTML/* downloads/GIF/* downloads/JavaScript/* downloads/Images/* src/*.o $(EXE)

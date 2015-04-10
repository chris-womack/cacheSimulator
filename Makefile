SHELL = /bin/sh

# the compiler: gcc for C program, define as g++ for C++
CC = gcc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall

# the build target executable:
TARGET = cacheSim

TAR1 = main
TAR2 = L1
TAR3 = L2
TAR4 = gen
TAR5 = configlib

all: $(TAR1)

$(TAR1): $(TAR1).c $(TAR2).c $(TAR3).c $(TAR4).c $(TAR5).c
	$(CC) $(TAR1).c $(CFLAGS) $(TAR2).c $(TAR3).c $(TAR4).c $(TAR5).c -o $(TARGET)

clean:
	$(RM) -rf *o $(TARGET)
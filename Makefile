#ECE373
#Assignment 4

#This is the makefile for the Assignment 4
#it compiles the file A4.c and ledmon.c  


#The compiler: gcc
CC = gcc


#compiler flags:
# -g adds debugging information to the executable file
#  -Wall turns on most, but not all , compiler warnings

TARGET = A4
TARGET1 = ledmon

all:$(TARGET)
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c


all:$(TARGET1)
$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c

clean:
	$(RM) $(TARGET1) $(TARGET)

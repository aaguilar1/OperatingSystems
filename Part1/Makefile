#Makefile. You can make changes to this file according to your need
CC = gcc
CFLAGS = -Wall -g 
LDFLAGS = -pthread

OBJS = Problem1.o Problem1b.o 

all: Project

Project: $(OBJS)

Problem1.o: Problem1.c
	 $(CC) $(LDFLAGS) Problem1.c -o p1

Problem1b.o: Problem1b.c
	 $(CC) $(LDFLAGS) Problem1b.c -o p1b
clean:
	rm -f *~ *.o project 
	

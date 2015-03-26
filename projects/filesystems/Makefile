
#John Brady & Albert Owusu-Asare
#Boxes 3119 & 4497
#December 8, 2014
#CSC213, Weinman
#Lab 10: Filesystems
# This document contains compile commands for the makefile

CC = gcc
CCOPTS = -c -g -Wall -ggdb 
LINKOPTS = -g
PROGRAMS = mycp myrm mytouch mymv myls

all: $(PROGRAMS)

# Implicit rule: Make program named x from x.c
% :: %.c
	$(CC) $(LINKOPTS) -o $@ $^

clean:
	rm -rf *.o *~ *.err *.out $(PROGRAMS)

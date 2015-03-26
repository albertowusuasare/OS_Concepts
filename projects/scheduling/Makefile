# Makefile for CSC213 Scheduling Lab.
# Originally written by Janet Davis
# Revised by Jerod Weinman, 8 August 2014
# Modified by Albert Owusu-Asare, 19 Sep 2014

# To compile with time quantum 10 and scheduler overhead 0.5, use
# make -B QUANTUM=10.0 OVERHEAD=0.5
QUANTUM = 0.1
OVERHEAD = 0.03
NUM_PRIORITY_LEVELS = 5

# These flags control the amount of debugging output.
DEBUG_FLAGS = # -DD_INPUT # -DD_EVENTLIST -DD_PRINTSTATS
#DEBUG_FLAGS =

# All programs should be listed here for use in 'make all' and 'make clean'.
PROGRAMS = sim_fcfs sim_sjn sim_rr sim_mlq

CC = gcc
CCOPTS = -c -Wall
LINKOPTS = -lm

all: $(PROGRAMS) 

sim_fcfs: scheduler_fcfs.o nonpreemptive_scheduler_simulation.o eventq.o stats.o
	$(CC) $(LINKOPTS) -o $@ $^

sim_sjn: scheduler_sjn.o nonpreemptive_scheduler_simulation.o eventq.o stats.o
	$(CC) $(LINKOPTS) -o $@ $^

sim_rr: scheduler_fcfs.o rr_scheduler_simulation.o eventq.o stats.o
	$(CC) $(LINKOPTS) -o $@ $^

sim_mlq: scheduler_mlq.o mlq_scheduler_simulation.o eventq.o stats.o
	$(CC) $(LINKOPTS) -DNUM_PRIORITY_LEVELS=$(NUM_PRIORITY_LEVELS) -o $@ $^

scheduler_fcfs.o: scheduler_fcfs.c scheduler.h
	$(CC) $(CCOPTS) -o $@ $<

scheduler_sjn.o: scheduler_sjn.c scheduler.h
	$(CC) $(CCOPTS) -o $@ $<

scheduler_mlq.o: scheduler_mlq.c scheduler.h
	$(CC) $(CCOPTS) -DNUM_PRIORITY_LEVELS=$(NUM_PRIORITY_LEVELS) -o $@ $<

nonpreemptive_scheduler_simulation.o: scheduler_simulation.c scheduler.h eventq.h stats.h
	$(CC) $(CCOPTS) $(DEBUG_FLAGS) -DOVERHEAD=$(OVERHEAD) -DQUANTUM=0.0 -o $@ $<

rr_scheduler_simulation.o: scheduler_simulation.c scheduler.h eventq.h stats.h
	$(CC) $(CCOPTS) $(DEBUG_FLAGS) -DOVERHEAD=$(OVERHEAD) -DQUANTUM=$(QUANTUM) -o $@ $<

mlq_scheduler_simulation.o: scheduler_simulation.c scheduler.h eventq.h stats.h
	$(CC) $(CCOPTS) $(DEBUG_FLAGS) \
	-DOVERHEAD=$(OVERHEAD) -DQUANTUM=$(QUANTUM) \
	-DNUM_PRIORITY_LEVELS=$(NUM_PRIORITY_LEVELS) -o $@ $<

eventq.o: eventq.c eventq.h
	$(CC) $(CCOPTS) -o $@ $<

stats.o: stats.c stats.h
	$(CC) $(CCOPTS) -o $@ $<

clean:
	rm -rf *.o *~ $(PROGRAMS)

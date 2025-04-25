CC=gcc
CFLAGS=-Wall -g

OBJS=main.o process_generator_impl.o scheduler_impl.o metrics_impl.o

all: simulator

simulator: $(OBJS)
	$(CC) -o simulator $(OBJS) -lm

main.o: prob_sched_simulator.c
	$(CC) $(CFLAGS) -c prob_sched_simulator.c -o main.o

process_generator_impl.o: process_generator_impl.c
	$(CC) $(CFLAGS) -c process_generator_impl.c

scheduler_impl.o: scheduler_impl.c
	$(CC) $(CFLAGS) -c scheduler_impl.c

metrics_impl.o: metrics_impl.c
	$(CC) $(CFLAGS) -c metrics_impl.c

clean:
	rm -f *.o simulator
CC=gcc
CFLAGS=-Wall -Wno-unused-value

caltrain: caltrain-runner.c caltrain.c caltrain.h
	$(CC) $(CFLAGS) -o caltrain caltrain-runner.c caltrain.c caltrain.h -lpthread

clean:
	rm  caltrain
/*
 * This file tests the implementation in caltrain.c. You shouldn't need to
 * modify it, but you're welcome to do so. Please report any bugs to us via
 * Piazza or email (cs140ta@cs).
 *
 * Note that passing these tests doesn't guarantee that your code is correct
 * or meets the specifications given, but hopefully it's at least pretty
 * close.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#include "caltrain.h"

// Count of passenger threads that have completed (i.e. station_wait_for_train
// has returned) and are awaiting a station_on_board() invocation.
volatile int threads_completed = 0;

void*
passenger_thread(void *arg)
{
	struct station *station = (struct station*)arg;
	station_wait_for_train(station);//assume threads sleep here
	__sync_add_and_fetch(&threads_completed, 1);
	return NULL;
}

struct load_train_args {
	struct station *station;
	int free_seats;
};

volatile int load_train_returned = 0;

void*
load_train_thread(void *args)
{
	struct load_train_args *ltargs = (struct load_train_args*)args;
	station_load_train(ltargs->station, ltargs->free_seats);
	load_train_returned = 1;
	return NULL;
}

const char* alarm_error_str;
int alarm_timeout;

void
_alarm(int seconds, const char *error_str)
{
	alarm_timeout = seconds;
	alarm_error_str = error_str;
	alarm(seconds);
}

void
alarm_handler(int foo)
{
	fprintf(stderr, "Error: Failed to complete after %d seconds. Something's "
		"wrong, or your system is terribly slow. Possible error hint: [%s]\n",
		alarm_timeout, alarm_error_str);
	exit(1);
}

#ifndef MIN
#define MIN(_x,_y) ((_x) < (_y)) ? (_x) : (_y)
#endif

/*
 * This creates a bunch of threads to simulate arriving trains and passengers.
 */
int
main()
{
	struct station station;
	station_init(&station);

	srandom(getpid() ^ time(NULL));

	signal(SIGALRM, alarm_handler);

	// Make sure station_load_train() returns immediately if no waiting passengers.
	_alarm(1, "station_load_train() did not return immediately when no waiting passengers");
	station_load_train(&station, 0);
	station_load_train(&station, 10);
	_alarm(0, NULL);

	// Create a bunch of 'passengers', each in their own thread.
	int i;
	const int total_passengers = 100;
	int passengers_left = total_passengers;
	for (i = 0; i < total_passengers; i++) {
		pthread_t tid;
		int ret = pthread_create(&tid, NULL, passenger_thread, &station);
		if (ret != 0) {
			// If this fails, perhaps we exceeded some system limit.
			// Try reducing 'total_passengers'.
			perror("pthread_create");
			exit(1);
		}
	}

	// Make sure station_load_train() returns immediately if no free seats.
	_alarm(2, "station_load_train() did not return immediately when no free seats");
	station_load_train(&station, 0);
	_alarm(0, NULL);

	// Tons of random tests.
	int total_passengers_boarded = 0;
	const int max_free_seats_per_train = 50;
	int pass = 0;
	while (passengers_left > 0) {
		_alarm(2, "Some more complicated issue appears to have caused passengers "
			"not to board when given the opportunity");

		int free_seats = random() % max_free_seats_per_train;

		printf("Train entering station with %d free seats\n", free_seats);
		load_train_returned = 0;
		struct load_train_args args = { &station, free_seats };
		pthread_t lt_tid;
		int ret = pthread_create(&lt_tid, NULL, load_train_thread, &args);
		if (ret != 0) {
			perror("pthread_create");
			exit(1);
		}

		int threads_to_reap = MIN(passengers_left, free_seats);
		int threads_reaped = 0;
		while (threads_reaped < threads_to_reap) {
			if (load_train_returned) {
				fprintf(stderr, "Error: station_load_train returned early!\n");
				exit(1);
			}
			if (threads_completed > 0) {
				if ((pass % 2) == 0)
					usleep(random() % 2);
				threads_reaped++;
				station_on_board(&station);
				__sync_sub_and_fetch(&threads_completed, 1);
			}
		}

		// Wait a little bit longer. Give station_load_train() a chance to return
		// and ensure that no additional passengers board the train. One second
		// should be tons of time, but if you're on a horribly overloaded system,
		// this may need to be tweaked.
		for (i = 0; i < 1000; i++) {
			if (i > 50 && load_train_returned)
				break;
			usleep(1000);
		}

		if (!load_train_returned) {
			fprintf(stderr, "Error: station_load_train failed to return\n");
			exit(1);
		}

		while (threads_completed > 0) {
			threads_reaped++;
			__sync_sub_and_fetch(&threads_completed, 1);
		}

		passengers_left -= threads_reaped;
		total_passengers_boarded += threads_reaped;
		printf("Train departed station with %d new passenger(s) (expected %d)%s\n",
			threads_reaped, threads_to_reap,
			(threads_to_reap != threads_reaped) ? " *****" : "");

		if (threads_to_reap != threads_reaped) {
			fprintf(stderr, "Error: Too many passengers on this train!\n");
			exit(1);
		}

		pass++;
	}

	if (total_passengers_boarded == total_passengers) {
		printf("Looks good!\n");
		return 0;
	} else {
		// I don't think this is reachable, but just in case.
		fprintf(stderr, "Error: expected %d total boarded passengers, but got %d!\n",
			total_passengers, total_passengers_boarded);
		return 1;
	}
}

/*******************************************************************************
 * CPU Clock Measurement Using RDTSC
 *
 * Description:
 *     This C file provides functions to compute and measure the CPU clock using
 *     the `rdtsc` instruction. The `rdtsc` instruction returns the Time Stamp
 *     Counter, which can be used to measure CPU clock cycles.
 *
 * Author:
 *     Renato Mancuso
 *
 * Affiliation:
 *     Boston University
 *
 * Creation Date:
 *     September 10, 2023
 *
 * Notes:
 *     Ensure that the platform supports the `rdtsc` instruction before using
 *     these functions. Depending on the CPU architecture and power-saving
 *     modes, the results might vary. Always refer to the CPU's official
 *     documentation for accurate interpretations.
 *
 *******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

#include "timelib.h"

int main(int argc, char **argv)
{
	/* IMPLEMENT ME! */

	if (argc != 4)
	{
		printf("Usage: %s <seconds> <nanoseconds> <method (s/b)>\n", argv[0]);
		return 1;
	}

	int seconds = atoi(argv[1]);
	int nanoseconds = atoi(argv[2]);
	char method = argv[3][0];

	if (method != 's' && method != 'b')
	{
		printf("Invalid method. Use 's' for sleep-based or 'b' for busy-waiting.\n");
		return 1;
	}

	long elapsed_cycles;
	if (method == 's')
	{
		elapsed_cycles = get_elapsed_sleep(seconds, nanoseconds);
	}
	else
	{
		elapsed_cycles = get_elapsed_busywait(seconds, nanoseconds);
	}

	double wait_time_in_seconds = seconds + (double)nanoseconds / 1e9;
	double clock_speed_MHz = elapsed_cycles / wait_time_in_seconds / 1e6;

	printf("WaitMethod: %s ", (method == 's') ? "SLEEP" : "BUSYWAIT");
	printf("WaitTime: %d %d ", seconds, nanoseconds);
	printf("ClocksElapsed: %llu ", (long long)elapsed_cycles);
	printf("ClockSpeed: %.2lf\n", clock_speed_MHz);

	return EXIT_SUCCESS;
}

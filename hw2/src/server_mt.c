/*******************************************************************************
 * Simple FIFO Order Server Implementation
 *
 * Description:
 *     A server implementation designed to process client requests in First In,
 *     First Out (FIFO) order. The server binds to the specified port number
 *     provided as a parameter upon launch.
 *
 * Usage:
 *     <build directory>/server <port_number>
 *
 * Parameters:
 *     port_number - The port number to bind the server to.
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
 *     Ensure to have proper permissions and available port before running the
 *     server. The server relies on a FIFO mechanism to handle requests, thus
 *     guaranteeing the order of processing. For debugging or more details, refer
 *     to the accompanying documentation and logs.
 *
 *******************************************************************************/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>

/* Needed for wait(...) */
#include <sys/types.h>
#include <sys/wait.h>

/* Include struct definitions and other libraries that need to be
 * included by both client and server */
#include "common.h"

#define BACKLOG_COUNT 100
#define USAGE_STRING                \
	"Missing parameter. Exiting.\n" \
	"Usage: %s <port_number>\n"

/* 4KB of stack for the worker thread */
#define STACK_SIZE (4096)

/* Main logic of the worker thread */
/* IMPLEMENT HERE THE MAIN FUNCTION OF THE WORKER */

int worker_main(void *arg)
{
	struct timespec ts;
	(void)arg; // unused parameter

	clock_gettime(CLOCK_MONOTONIC, &ts);
	printf("[#WORKER#] %.6lf Worker Thread Alive!\n", ts.tv_sec + ts.tv_nsec / 1e9);

	while (1)
	{
		get_elapsed_busywait(1, 0);
		clock_gettime(CLOCK_MONOTONIC, &ts);
		printf("[#WORKER#] %.6lf Still Alive!\n", ts.tv_sec + ts.tv_nsec / 1e9);
		// sleep for 1 second
		sleep(1);
	}

	return 0;
}

/* Main function to handle connection with the client. This function
 * takes in input conn_socket and returns only when the connection
 * with the client is interrupted. */
void handle_connection(int conn_socket)
{

	/* The connection with the client is alive here. Let's start
	 * the worker thread. */

	/* IMPLEMENT HERE THE LOGIC TO START THE WORKER THREAD. */

	/* We are ready to proceed with the rest of the request
	 * handling logic. */

	/* REUSE LOGIC FROM HW1 TO HANDLE THE PACKETS */

	/* IMPLEMENT ME! */

	unsigned long long request_id;
	struct timespec sent_timestamp, request_length, receipt_timestamp, completion_timestamp;

	char *stack = malloc(STACK_SIZE);
	if (!stack)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	/* Create the child thread */
	if (clone(worker_main, stack + STACK_SIZE, CLONE_THREAD | CLONE_VM | CLONE_SIGHAND | CLONE_FS | CLONE_FILES | CLONE_SYSVSEM, NULL) == -1)
	{
		perror("Clone failed");
		free(stack);
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		int bytes_read = read(conn_socket, &request_id, sizeof(request_id));
		bytes_read += read(conn_socket, &sent_timestamp, sizeof(sent_timestamp));
		bytes_read += read(conn_socket, &request_length, sizeof(request_length));

		if (bytes_read <= 0)
		{
			printf("INFO: Client disconnected. Exiting.\n");
			close(conn_socket);
			return;
		}

		clock_gettime(CLOCK_MONOTONIC, &receipt_timestamp);
		get_elapsed_busywait(request_length.tv_sec, request_length.tv_nsec);
		clock_gettime(CLOCK_MONOTONIC, &completion_timestamp);

		write(conn_socket, &request_id, sizeof(request_id));

		printf("R%llu:%ld.%06ld,%ld.%06ld,%ld.%06ld,%ld.%06ld\n",
			   request_id,
			   sent_timestamp.tv_sec, sent_timestamp.tv_nsec / 1000,
			   request_length.tv_sec, request_length.tv_nsec / 1000,
			   receipt_timestamp.tv_sec, receipt_timestamp.tv_nsec / 1000,
			   completion_timestamp.tv_sec, completion_timestamp.tv_nsec / 1000);
	}
}

/* Template implementation of the main function for the FIFO
 * server. The server must accept in input a command line parameter
 * with the <port number> to bind the server to. */
int main(int argc, char **argv)
{
	int sockfd, retval, accepted, optval;
	in_port_t socket_port;
	struct sockaddr_in addr, client;
	struct in_addr any_address;
	socklen_t client_len;

	/* Get port to bind our socket to */
	if (argc > 1)
	{
		socket_port = strtol(argv[1], NULL, 10);
		printf("INFO: setting server port as: %d\n", socket_port);
	}
	else
	{
		ERROR_INFO();
		fprintf(stderr, USAGE_STRING, argv[0]);
		return EXIT_FAILURE;
	}

	/* Now onward to create the right type of socket */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		ERROR_INFO();
		perror("Unable to create socket");
		return EXIT_FAILURE;
	}

	/* Before moving forward, set socket to reuse address */
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval));

	/* Convert INADDR_ANY into network byte order */
	any_address.s_addr = htonl(INADDR_ANY);

	/* Time to bind the socket to the right port  */
	addr.sin_family = AF_INET;
	addr.sin_port = htons(socket_port);
	addr.sin_addr = any_address;

	/* Attempt to bind the socket with the given parameters */
	retval = bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in));

	if (retval < 0)
	{
		ERROR_INFO();
		perror("Unable to bind socket");
		return EXIT_FAILURE;
	}

	/* Let us now proceed to set the server to listen on the selected port */
	retval = listen(sockfd, BACKLOG_COUNT);

	if (retval < 0)
	{
		ERROR_INFO();
		perror("Unable to listen on socket");
		return EXIT_FAILURE;
	}

	/* Ready to accept connections! */
	printf("INFO: Waiting for incoming connection...\n");
	client_len = sizeof(struct sockaddr_in);
	accepted = accept(sockfd, (struct sockaddr *)&client, &client_len);

	if (accepted == -1)
	{
		ERROR_INFO();
		perror("Unable to accept connections");
		return EXIT_FAILURE;
	}

	/* Ready to handle the new connection with the client. */
	handle_connection(accepted);

	close(sockfd);
	return EXIT_SUCCESS;
}

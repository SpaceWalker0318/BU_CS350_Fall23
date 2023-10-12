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
#include <sched.h>
#include <signal.h>

/* Needed for wait(...) */
#include <sys/types.h>
#include <sys/wait.h>

/* Needed for semaphores */
#include <semaphore.h>

/* Include struct definitions and other libraries that need to be
 * included by both client and server */
#include "common.h"

#define BACKLOG_COUNT 100
#define USAGE_STRING                \
	"Missing parameter. Exiting.\n" \
	"Usage: %s <port_number>\n"

/* 4KB of stack for the worker thread */
#define STACK_SIZE (4096)

/* START - Variables needed to protect the shared queue. DO NOT TOUCH */
sem_t *queue_mutex;
sem_t *queue_notify;
/* END - Variables needed to protect the shared queue. DO NOT TOUCH */

/* Max number of requests that can be queued */
#define QUEUE_SIZE 500

struct extended_request
{
	struct request req;
	struct timespec receipt_timestamp;
};

struct Node
{
	struct extended_request ext_req;
	struct Node *next;
};

struct queue
{
	/* IMPLEMENT ME */
	struct Node *front;
	struct Node *rear;
	int size;
};

struct worker_params
{
	/* IMPLEMENT ME */
	struct queue *the_queue;
	int conn_socket;
};

/* Implement this method to correctly dump the status of the queue
 * following the format Q:[R<request ID>,R<request ID>,...] */
void dump_queue_status(struct queue *the_queue)
{
	int i;
	/* QUEUE PROTECTION INTRO START --- DO NOT TOUCH */
	sem_wait(queue_mutex);
	/* QUEUE PROTECTION INTRO END --- DO NOT TOUCH */

	/* WRITE YOUR CODE HERE! */
	/* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! */
	printf("Q:[");
	struct Node *current = the_queue->front;
	while (current != NULL)
	{
		printf("R%llu", current->ext_req.req.req_id);
		if (current->next != NULL)
			printf(",");
		current = current->next;
	}
	printf("]\n");

	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
	sem_post(queue_mutex);
	/* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH */
}

/* Add a new request <request> to the shared queue <the_queue> */
int add_to_queue(struct extended_request to_add, struct queue *the_queue)
{
	int retval = 0;
	/* QUEUE PROTECTION INTRO START --- DO NOT TOUCH */
	sem_wait(queue_mutex);
	/* QUEUE PROTECTION INTRO END --- DO NOT TOUCH */

	/* WRITE YOUR CODE HERE! */
	/* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! */
	struct Node *newNode = (struct Node *)malloc(sizeof(struct Node));
	newNode->ext_req = to_add;
	newNode->next = NULL;
	if (the_queue->size >= QUEUE_SIZE)
	{
		retval = -1; // return -1 if the queue is full
		goto OUTRO;
	}

	if (the_queue->front == NULL && the_queue->rear == NULL)
	{
		the_queue->front = newNode;
		the_queue->rear = newNode;
	}
	else
	{
		the_queue->rear->next = newNode;
		the_queue->rear = newNode;
	}
	the_queue->size++;

OUTRO:
	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
	sem_post(queue_mutex);
	sem_post(queue_notify);
	/* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH */
	return retval;
}

/* Add a new request <request> to the shared queue <the_queue> */
struct extended_request get_from_queue(struct queue *the_queue)
{
	struct extended_request retval;
	retval.req.req_id = -1; // Assume req_id is signed, -1 represents an empty queue
	/* QUEUE PROTECTION INTRO START --- DO NOT TOUCH */
	sem_wait(queue_notify);
	sem_wait(queue_mutex);
	/* QUEUE PROTECTION INTRO END --- DO NOT TOUCH */

	/* WRITE YOUR CODE HERE! */
	/* MAKE SURE NOT TO RETURN WITHOUT GOING THROUGH THE OUTRO CODE! */
	if (the_queue->front == NULL)
	{
		// handle appropriately, perhaps by returning an error request or empty request.
		goto OUTRO;
	}
	struct Node *current = the_queue->front;
	retval = current->ext_req;
	the_queue->front = current->next;

	if (the_queue->front == NULL)
	{
		// If front is NULL, then the queue is empty, so rear should also be NULL
		the_queue->rear = NULL;
	}
	free(current);
	the_queue->size--;
OUTRO:
	/* QUEUE PROTECTION OUTRO START --- DO NOT TOUCH */
	sem_post(queue_mutex);
	/* QUEUE PROTECTION OUTRO END --- DO NOT TOUCH */
	return retval;
}

/* Main logic of the worker thread */
/* IMPLEMENT HERE THE MAIN FUNCTION OF THE WORKER */
int worker_main(void *arg)
{
	struct worker_params *params = (struct worker_params *)arg;
	struct queue *the_queue = params->the_queue;
	int conn_socket = params->conn_socket;

	unsigned long long request_id;
	struct timespec sent_timestamp, request_length, receipt_timestamp, start_timestamp, completion_timestamp;

	while (1)
	{
		struct extended_request ext_req = get_from_queue(the_queue);
		clock_gettime(CLOCK_MONOTONIC, &start_timestamp);
		receipt_timestamp = ext_req.receipt_timestamp;
		struct request req = ext_req.req;
		if (req.req_id == -1)
		{
			continue;
		}
		request_id = req.req_id;
		sent_timestamp = req.req_timestamp;
		request_length = req.req_length;
		busywait_timespec(request_length);
		clock_gettime(CLOCK_MONOTONIC, &completion_timestamp);

		write(conn_socket, &request_id, sizeof(request_id));

		printf("R%llu:%ld.%06ld,%ld.%06ld,%ld.%06ld,%ld.%06ld,%ld.%06ld\n",
			   request_id,
			   sent_timestamp.tv_sec, sent_timestamp.tv_nsec / 1000,
			   request_length.tv_sec, request_length.tv_nsec / 1000,
			   receipt_timestamp.tv_sec, receipt_timestamp.tv_nsec / 1000,
			   start_timestamp.tv_sec, start_timestamp.tv_nsec / 1000,
			   completion_timestamp.tv_sec, completion_timestamp.tv_nsec / 1000);
		dump_queue_status(the_queue);
	}

	return 0;
}

/* Main function to handle connection with the client. This function
 * takes in input conn_socket and returns only when the connection
 * with the client is interrupted. */
void handle_connection(int conn_socket)
{
	struct request *req;
	struct queue *the_queue;
	size_t in_bytes;

	/* The connection with the client is alive here. Let's
	 * initialize the shared queue. */

	/* IMPLEMENT HERE ANY QUEUE INITIALIZATION LOGIC */
	printf("Initializing queue\n");
	the_queue = (struct queue *)malloc(sizeof(struct queue));
	the_queue->front = NULL;
	the_queue->rear = NULL;
	the_queue->size = 0;

	struct worker_params *params = (struct worker_params *)malloc(sizeof(struct worker_params));
	params->the_queue = the_queue;
	params->conn_socket = conn_socket;

	/* Queue ready to go here. Let's start the worker thread. */

	/* IMPLEMENT HERE THE LOGIC TO START THE WORKER THREAD. */
	void *worker_stack = malloc(STACK_SIZE);
	if (!worker_stack)
	{
		perror("Unable to allocate memory for worker stack");
		exit(EXIT_FAILURE);
	}

	int thread_id = clone(worker_main, (char *)worker_stack + STACK_SIZE,
						  CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM,
						  params);
	if (thread_id == -1)
	{
		perror("Unable to create worker thread");
		free(worker_stack);
		exit(EXIT_FAILURE);
	}

	/* We are ready to proceed with the rest of the request
	 * handling logic. */

	/* REUSE LOGIC FROM HW1 TO HANDLE THE PACKETS */

	req = (struct request *)malloc(sizeof(struct request));
	if (!req)
	{
		perror("Unable to allocate memory for request");
		exit(EXIT_FAILURE);
	}

	do
	{
		in_bytes = recv(conn_socket, req, sizeof(struct request), 0);
		/* SAMPLE receipt_timestamp HERE */

		/* Don't just return if in_bytes is 0 or -1. Instead
		 * skip the response and break out of the loop in an
		 * orderly fashion so that we can de-allocate the req
		 * and resp varaibles, and shutdown the socket. */
		if (in_bytes > 0)
		{
			struct extended_request ext_req;
			ext_req.req = *req;
			clock_gettime(CLOCK_MONOTONIC, &ext_req.receipt_timestamp);
			int add_result = add_to_queue(ext_req, the_queue);
		}
	} while (in_bytes > 0);

	/* PERFORM ORDERLY DEALLOCATION AND OUTRO HERE */
	free(req);
	free(the_queue);
	free(worker_stack);
	close(conn_socket);

	/* Wait for the worker thread to finish */
	waitpid((pid_t)thread_id, NULL, 0);

	return;
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

	/* Initialize queue protection variables. DO NOT TOUCH. */
	queue_mutex = (sem_t *)malloc(sizeof(sem_t));
	queue_notify = (sem_t *)malloc(sizeof(sem_t));
	retval = sem_init(queue_mutex, 0, 1);
	if (retval < 0)
	{
		ERROR_INFO();
		perror("Unable to initialize queue mutex");
		return EXIT_FAILURE;
	}
	retval = sem_init(queue_notify, 0, 0);
	if (retval < 0)
	{
		ERROR_INFO();
		perror("Unable to initialize queue notify");
		return EXIT_FAILURE;
	}
	/* DONE - Initialize queue protection variables. DO NOT TOUCH */

	/* Ready to handle the new connection with the client. */
	handle_connection(accepted);

	free(queue_mutex);
	free(queue_notify);

	close(sockfd);
	return EXIT_SUCCESS;
}

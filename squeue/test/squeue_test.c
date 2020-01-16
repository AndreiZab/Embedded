#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include <squeue.h>

#define LEN_DATA 16

#define SIZE_PREFILL 2

typedef struct {
	uint8_t data[LEN_DATA];
}
item_t;

static void queue_dump(squeue_t *queue)
{
	uint32_t i = 0;

	printf("head %u tail %u", queue->p_head, queue->p_tail);
	for ( i = 0; i < SQUEUE_LEN; i++ ) {
		printf("  %0u:%02x", i, queue->element[i].data[0]);
	}
	printf("\n");
}

int main(void)
{
	int retcode = 0;
	uint32_t i = 0;
	item_t item;
	assert(LEN_DATA == SQUEUE_ELEMENT_SIZE);

	squeue_t queue;

	squeue_init(&queue);

	for ( i = 0; i < SIZE_PREFILL; i++) {
		item.data[0] = 0xFF;
		retcode = squeue_enqueue(&queue, (squeue_element_t*)&item);
		if ( retcode != 0 ) {
			printf("Couldn't enqueue\n");
			break;
		}

		//printf("Enqueued %02x\n", item.data[0]);
	}

	queue_dump(&queue);
	if (( queue.p_head != 0) ||
		( queue.p_tail != SIZE_PREFILL ) ||
		( queue.element[0].data[0] != 0xFF ) ||
		( queue.element[SIZE_PREFILL - 1].data[0] != 0xFF )) {
		return -1;
	}

	retcode = 0;
	while ( true ) {
		retcode = squeue_dequeue(&queue, (squeue_element_t*)&item);
		if ( retcode != 0 ) {
			//printf("Couldn't dequeue\n");
			break;
		}

		//printf("Dequeued %02x\n", item.data[0]);
	}

	queue_dump(&queue);
	if (( queue.p_head != SIZE_PREFILL) ||
		( queue.p_tail != SIZE_PREFILL ) ||
		( queue.element[0].data[0] != 0x0 ) ||
		( queue.element[SIZE_PREFILL - 1].data[0] != 0x0 )) {
		return -1;
	}

	retcode = 0;
	item.data[0] = 0x11;
	while ( true ) {
		retcode = squeue_enqueue(&queue, (squeue_element_t*)&item);
		if ( retcode != 0 ) {
			//printf("Couldn't enqueue\n");
			break;
		}

		//printf("Enqueued %02x\n", item.data[0]);
		item.data[0]++;
	}

	queue_dump(&queue);
	if (( queue.p_head != SIZE_PREFILL) ||
		( queue.p_tail != (( SIZE_PREFILL - 1 ) % SQUEUE_LEN )) ||
		( queue.element[SIZE_PREFILL].data[0] != 0x11 ) ||
		( queue.element[( SIZE_PREFILL - 2 ) % SQUEUE_LEN].data[0] != ( 0x11 + SQUEUE_LEN - 2))) {
		return -1;
	}

	retcode = 0;
	while ( true ) {
		retcode = squeue_dequeue(&queue, (squeue_element_t*)&item);
		if ( retcode != 0 ) {
			//printf("Couldn't dequeue\n");
			break;
		}

		//printf("Dequeued %02x\n", item.data[0]);
	}

	queue_dump(&queue);
	if (( queue.p_head != (( SIZE_PREFILL - 1 ) % SQUEUE_LEN )) ||
		( queue.p_tail != (( SIZE_PREFILL - 1 ) % SQUEUE_LEN )) ||
		( queue.element[SIZE_PREFILL].data[0] != 0x00 ) ||
		( queue.element[( SIZE_PREFILL - 2 ) % SQUEUE_LEN].data[0] != 0x00 )) {
		return -1;
	}

	return 0;
}

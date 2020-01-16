/** @file squeue.c
	@brief Lightweight queue implementation on static circular buffers.

	@par Design Objectives
	-   Minimal CPU overhead. The module is supposed to be extensively used in irq context.
	-   Build-in safety mechanisms:
	    -   protective buffers cleanup

	@par Limitations
	-   Queue depth is fixed.
	-   Actual queue depth is less by one that the declared SQUEUE_LEN.
	-   The implementation is not thread safe.
	-   Queue elements have fixed size. It is the callers responsibility to assert size matching.
*/
#include <string.h>
#include <stdint.h>

#ifdef ARCH_AT32
	#include <compiler.h>
#endif

#include <squeue.h>

/** Module API: Queue initializer
 *******************************************************************************
 @par Action
 -   Brings the given queue to the default safe state
 
 @param[in] queue - pointer to the queue being initialized
 ******************************************************************************/
void squeue_init(squeue_t* queue)
{
	memset((uint8_t*)queue, 0, sizeof(squeue_t));
}

/** Module API: retrieval method
 *******************************************************************************
 @par Action
 -   Fills the provided storage with the head element content
 -   Removes the head element from the queue

 @par NB
 -   The storage provided will be zero-filled (safestated) even if the queue is empty.

 @param[in] queue - pointer to the queue
 @param[in] queue - pointer to the element

 @return Operation status
 -   0: success
 -   -1: failure
 ******************************************************************************/
int squeue_dequeue(squeue_t* queue, squeue_element_t* element_output)
{
	// Safestate the storage provided
	memset(element_output->data, 0, sizeof(squeue_element_t));

	// Check if the queue is empty, safestate the storage provided
	if ( queue->p_tail == queue->p_head) {
		return -1;
	}

	// Retrieve data into the storage provided
	memcpy(element_output->data, queue->element[queue->p_head].data, sizeof(squeue_element_t));

	// Safestate the queue element
	memset(queue->element[queue->p_head].data, 0, sizeof(squeue_element_t));

	// Move the head pointer
	queue->p_head = ( queue->p_head + 1 ) % SQUEUE_LEN;

	return 0;
}


/** Module API: insertion method
 *******************************************************************************
 @par Action
 -   Adds the given element to the given queue
 
 @param[in] queue - pointer to the queue
 @param[in] queue - pointer to the element

 @return Operation status
 -   0: success
 -   -1: failure
 ******************************************************************************/
int squeue_enqueue(squeue_t* queue, squeue_element_t* element_input)
{
	// Check if the queue is full. We sacrifice the last available 
	// circular buffer element for sake of simplicity
	if ( ( ( queue->p_head - queue->p_tail ) % SQUEUE_LEN ) == 1 ) {
		return -1;
	}

	// Insert data from the input buffer
	memcpy(queue->element[queue->p_tail].data, element_input->data, sizeof(squeue_element_t));

	// Update the tail pointer without additional protective measures
	// as we do not use the last available circular buffer element.
	queue->p_tail = ( queue->p_tail + 1 ) % SQUEUE_LEN;

	return 0;
}

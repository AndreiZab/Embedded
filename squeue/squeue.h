#define SQUEUE_LEN 8
#define SQUEUE_ELEMENT_SIZE 16

typedef struct {
	uint8_t data[SQUEUE_ELEMENT_SIZE];
}
squeue_element_t;

typedef struct {
	uint32_t p_head;
	uint32_t p_tail;
	squeue_element_t element[SQUEUE_LEN];
}
squeue_t;

void squeue_init(squeue_t* queue);
int squeue_dequeue(squeue_t* queue, squeue_element_t* element_output);
int squeue_enqueue(squeue_t* queue, squeue_element_t* element_input);

//bool squeue_empty(squeue_t* queue);

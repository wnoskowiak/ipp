#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct coord
{
    size_t len;
    size_t *coord;
} coord_t;

typedef struct queue
{
    size_t front, back, cap, size;
    struct coord *array;
} queue_t;

queue_t *initialize_queue(int cap)
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    queue->cap = cap;
    queue->front = queue->size = 0;
    queue->back = cap - 1;
    queue->array = (struct coord *)calloc(cap, sizeof(struct coord));
    return queue;
}

bool isEmpty(queue_t *queue)
{
    return (queue->size == 0);
}

bool isFull(queue_t *queue)
{
    return (queue->size == queue->cap);
}

void add(queue_t *queue, coord_t item)
{
    if (isFull(queue))
    {
        return;
    }
    queue->back = (queue->back + 1) % queue->cap;
    queue->array[queue->back] = item;
    queue->size = queue->size + 1;
}

coord_t *pop(queue_t *queue)
{
    if (isEmpty(queue))
    {
        return NULL;
    }
    coord_t* res = &(queue->array[queue->front]);
    queue->front = (queue->front + 1)% queue->cap;
    queue->size = queue->size -1;
    return res;
}

int main(){}
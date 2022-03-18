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

queue_t * resize_queue(queue_t *queue)
{
    queue_t *narr = realloc(queue->array, (queue->cap + 2) * sizeof(struct coord));
    if (narr == NULL)
    {
        return NULL;
    }
    free(queue->array);
    queue->cap += 2;
    queue->array = narr;
    if (queue->back < queue->front)
    {
        for (int i = 0; i < queue->back; i++)
        {
            queue->array[(queue->size -2 + i) % queue->size] = queue->array[i];
        }
        queue->back = (queue->size -2 + queue->back) % queue->size;
    }
    return queue;
}

void queue_destroy(queue_t *queue)
{
    for(int i = 0; i<length(queue->array)-1; i++)
    {
        free((&(queue->array)[i])->coord);
        free(&(queue->array)[i]);
    }
    free(queue->array);
    free(queue);
}

bool isEmpty(queue_t *queue)
{
    return (queue->size == 0);
}

bool isFull(queue_t *queue)
{
    return (queue->size == queue->cap);
}

queue_t* add(queue_t *queue, coord_t item)
{
    if (isFull(queue))
    {
        queue_t* nqueue = resize_queue(queue);
        if (nqueue == NULL)
        {
            return NULL;
        }
        else
        {
            queue = nqueue;
        }
    }
    queue->back = (queue->back + 1) % queue->cap;
    queue->array[queue->back] = item;
    queue->size = queue->size + 1;
    return queue;
}

coord_t *pop(queue_t *queue)
{
    if (isEmpty(queue))
    {
        return NULL;
    }
    coord_t *res = &(queue->array[queue->front]);
    queue->front = (queue->front + 1) % queue->cap;
    queue->size = queue->size - 1;
    return res;
}
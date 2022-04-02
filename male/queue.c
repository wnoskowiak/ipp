#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include "types.h"

static void fail(int i)
{
    fprintf(stderr, "ERROR %i\n", i);
    exit(EXIT_FAILURE);
}

queue_t* queue_initialize(size_t cap)
{
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    queue->cap = cap;
    queue->front = queue->size = 0;
    queue->back = cap - 1;
    queue->array = (coord_t **)calloc(cap, sizeof(coord_t *));
    if(queue->array == NULL)
    {
        return NULL;
    }
    return queue;
}

void queue_destroy(queue_t *queue)
{
    coord_t* temp;
    while (!is_empty(queue))
    {
        temp = pop(queue);
        free(temp->coord.array);
        free(temp);
    }
    free(queue->array);
    free(queue);
}

queue_t* queue_resize(queue_t* queue)
{
    coord_t **narr = (coord_t **)realloc(queue->array, (queue->cap + 2) * sizeof(coord_t *));
    if (narr == NULL)
    {
        queue_destroy(queue);
        return NULL;
    }
    queue->cap += 2;
    queue->array = narr;
    if (queue->back < queue->front)
    {

        for (size_t i = 0; i < queue->back + 1; i++)
        {
            queue->array[(queue->cap - 2 + i) % queue->cap] = queue->array[i];
        }
        queue->back = (queue->cap - 2 + queue->back) % queue->cap;
    }
    return queue;
}

bool is_empty(queue_t *queue)
{
    return (queue->size == 0);
}

bool is_full(queue_t *queue)
{
    return (queue->size == queue->cap);
}

queue_t *add(queue_t* queue, coord_t *item)
{
    if (is_full(queue))
    {
        queue = queue_resize(queue);
        if(!queue)
        {
            return NULL;
        }
    }
    queue->back = (queue->back + 1) % queue->cap;
    queue->array[queue->back] = item;
    queue->size = queue->size + 1;
    return queue;
}

coord_t *pop(queue_t *queue)
{
    if (is_empty(queue))
    {
        return NULL;
    }
    coord_t *res = (queue->array[queue->front]);
    queue->front = (queue->front + 1) % queue->cap;
    queue->size = queue->size - 1;
    return res;
}
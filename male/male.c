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

queue_t *initialize_queue(size_t cap)
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    queue->cap = cap;
    queue->front = queue->size = 0;
    queue->back = cap - 1;
    queue->array = (struct coord *)calloc(cap, sizeof(struct coord));
    return queue;
}

queue_t *resize_queue(queue_t *queue)
{
    coord_t *narr = realloc(queue->array, (queue->cap + 2) * sizeof(struct coord));
    if (narr == NULL)
    {
        return NULL;
    }
    queue->cap += 2;
    queue->array = narr;
    if (queue->back < queue->front)
    {
        for (size_t i = 0; i < queue->back; i++)
        {
            queue->array[(queue->size - 2 + i) % queue->size] = queue->array[i];
        }
        queue->back = (queue->size - 2 + queue->back) % queue->size;
    }
    return queue;
}
void print_coord(coord_t r, size_t dim)
{
    printf("%zu\n", r.len);
    for (size_t i = 0; i < dim; i++)
    {
        printf("%zu", r.coord[i]);
        printf(" ");
    }
    printf("\n");
}
void queue_destroy(queue_t *queue)
{
    for (size_t i = 0; i < queue->cap; i++)
    {
        print_coord((queue->array[i]), 6);
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

queue_t *add(queue_t *queue, coord_t item)
{
    if (isFull(queue))
    {
        queue_t *nqueue = resize_queue(queue);
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

/* testy */

coord_t gen_coor(size_t dupa, size_t dim)
{
    coord_t a;
    a.len = 0;
    a.coord = (size_t *)calloc(dim, sizeof(size_t));
    for (size_t i = 0; i < dim; i++)
    {
        a.coord[i] = dupa;
    }
    return a;
}

int main()
{
    size_t dim = 6;
    queue_t *queue = initialize_queue(10);
    for (int i = 0; i < 13; i++)
    {
        add(queue, gen_coor(i, dim));
        add(queue, gen_coor(i + 1, dim));
        add(queue, gen_coor(i + 2, dim));
        add(queue, gen_coor(i + 3, dim));
        coord_t *a = pop(queue);
        print_coord(*a, dim);
        free(a->coord);
        coord_t *b = pop(queue);
        print_coord(*b, dim);
        free(b->coord);
        coord_t *c = pop(queue);
        print_coord(*c, dim);
        free(c->coord);
        coord_t *d = pop(queue);
        print_coord(*d, dim);
        free(d->coord);
    }
    printf("dupa2 \n");
    printf("%zu\n", queue->cap);
    add(queue, gen_coor(67, dim));
    queue_destroy(queue);
    printf("dupa3 \n");
}
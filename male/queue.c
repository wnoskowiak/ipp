#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <queue.h>

typedef struct coord
{
    size_t len;
    size_t *coord;
} coord_t;

typedef struct queue
{
    size_t front, back, cap, size;
    struct coord **array;
} queue_t;

queue_t *queue_initialize(size_t cap)
{
    queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
    queue->cap = cap;
    queue->front = queue->size = 0;
    queue->back = cap - 1;
    queue->array = (coord_t **)calloc(cap, sizeof(coord_t *));
    return queue;
}

queue_t *queue_resize(queue_t *queue)
{
    coord_t **narr = (coord_t **)realloc(queue->array, (queue->cap + 2) * sizeof(coord_t *));
    if (narr == NULL)
    {
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

queue_t *add(queue_t *queue, coord_t *item)
{
    if (is_full(queue))
    {
        queue_t *nqueue = queue_resize(queue);
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
    if (is_empty(queue))
    {
        return NULL;
    }
    coord_t *res = (queue->array[queue->front]);
    queue->front = (queue->front + 1) % queue->cap;
    queue->size = queue->size - 1;
    return res;
}

void queue_destroy(queue_t *queue)
{
    coord_t *temp = pop(queue);
    while (temp)
    {
        free(temp->coord);
        free(temp);
        temp = pop(queue);
    }
    free(queue->array);
    free(queue);
}

/* testy */

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

coord_t *gen_coor(size_t dupa, size_t dim)
{
    coord_t *a = (coord_t *)malloc(sizeof(coord_t));
    a->len = 0;
    a->coord = (size_t *)calloc(dim, sizeof(size_t));
    for (size_t i = 0; i < dim; i++)
    {
        a->coord[i] = dupa;
    }
    return a;
}

int main()
{
    size_t dim = 6;
    queue_t *queue = queue_initialize(3);
    for (int i = 0; i < 777; i++)
    {
        coord_t *w = gen_coor(100 * i, dim);
        add(queue, w);
        coord_t *f = gen_coor(100 * i+1, dim);
        add(queue, f);
        coord_t *y = gen_coor(100 * i+2, dim);
        add(queue, y);
        /*
        coord_t *e = pop(queue);
        print_coord(*e, dim);
        free(e->coord);
        free(e);*/
    }
        for (int i = 0; i < 77; i++)
    {
        coord_t *e = pop(queue);
        print_coord(*e, dim);
        free(e->coord);
        free(e);
    }

    printf("%zu\n", queue->cap);
    queue_destroy(queue);
}
#ifndef QUEUE_H_
#define QUEUE_H_
#include <stdlib.h>
#include <stdbool.h>

typedef struct coord{} coord_t;

typedef struct queue{} queue_t;

queue_t *queue_initialize(size_t cap){};

bool is_empty(queue_t *queue){};

bool is_full(queue_t *queue){};

queue_t *add(queue_t *queue, coord_t *item){};

coord_t *pop(queue_t *queue){};

void queue_destroy(queue_t *queue){};

#endif
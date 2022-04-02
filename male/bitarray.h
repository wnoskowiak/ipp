#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"

/*
zwykły bfs działa, ale trzeba nie spierdolić
labirynt można trzymać jako tablice intów
*/

#ifndef BITARRAY_H_
#define BITARRAY_H_

typedef struct settings settings_t;

typedef struct arr arr_t;

typedef struct indx indx_t;

typedef struct array_with_length array_with_length_t;

arr_t *initialize_array(array_with_length_t* dimention_definitions);

void destroy_array(arr_t *array);

indx_t *to_internal_type(arr_t *array, size_t *coords);

bool put(arr_t *array, size_t *coords);

signed char get(arr_t *array, size_t *coords);

void put_in_array(arr_t *array, indx_t *index);

#endif
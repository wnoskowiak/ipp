#include <stdio.h>
#include <stdbool.h>

#ifndef TYPES_H_
#define TYPES_H_

typedef struct indx
{
    size_t cell;
    size_t rem;
} indx_t;

typedef struct array_with_length
{
    size_t *array;
    size_t length;
} array_with_length_t;

typedef struct coord
{
    size_t len;
    array_with_length_t coord;
} coord_t;

typedef struct queue
{
    size_t front, back, cap, size;
    struct coord **array;
} queue_t;

typedef struct settings
{
    size_t *dimmaxes;
    size_t dimnum;
    size_t mx;
} settings_t;

typedef struct arr
{
    settings_t *settings;
    size_t *array;
} arr_t;

#endif
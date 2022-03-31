#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include "queue.h"

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

typedef struct array_with_length
{
    size_t *array;
    size_t length;
} array_with_length_t;

array_with_length_t *line_to_array(char *str)
{
    const char s[2] = " ";
    char *token;
    token = strtok(str, s);
    queue_t *dupa = queue_initialize(2);
    while (token != NULL)
    {
        size_t len = 0;
        sscanf(token, "%zu", &len);
        coord_t *a = (coord_t *)malloc(sizeof(coord_t));
        a->coord = (size_t *)malloc(sizeof(size_t));
        *(a->coord) = len;
        add(dupa, a);
        token = strtok(NULL, s);
    }
    array_with_length_t* res = (array_with_length_t*)malloc(sizeof(array_with_length_t));
    res->length = dupa->size;
    res->array = (size_t *)calloc(res->length, sizeof(size_t));
    for (size_t i = 0; i < res->length; i++)
    {
        coord_t *fuck = pop(dupa);
        res->array[i] = *(fuck->coord);
        free(fuck->coord);
        free(fuck);
    }
    // for (size_t i = 0; i < res->length; i++)
    // {
    //     printf("%zu\n", res->array[i]);
    // }
    // queue_destroy(dupa);
    return res;
}

int main()
{
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, stdin)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
    }
    
    //char str[5375] = "0000018446744073709551615     018446744073709551606        0000000000000000000000000000000000000000000000000000000000000000000000018 18446744073709551534 00018446744073709551499 0000018446744073709551521";
    array_with_length_t *ubuntu = line_to_array(line);
        for(size_t i = 0; i< ubuntu->length; i++)
    {
        printf("%zu\n",ubuntu->array[i]);
    }
    free(line);
    return 0;
}
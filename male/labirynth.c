#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include "queue.h"
#include "bitarray.h"

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

void parce_R(char *token)
{
    const char s[2] = " ";
    unsigned int res[5];
    int i;
    if (token++ != NULL)
    {
        sscanf(token, "%u", &res[0]);
        i = 1;
    }
    else
    {
        i = 0;
    }
    token = strtok(NULL, s);
    while (token != NULL)
    {
        sscanf(token, "%u", &res[i]);
        i++;
        token = strtok(NULL, s);
    }
    if (i < 5)
    {
        fwrite("ERROR 4\n", 8, 1, stderr);
        exit(EXIT_FAILURE);
    }
    // return res;
}

void parce_hex(char *token)
{
    printf("%s\n", token);
    size_t number = strtoul(token, NULL, 16);
    printf("%zu\n", number);
}

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
    array_with_length_t *res = (array_with_length_t *)malloc(sizeof(array_with_length_t));
    res->length = dupa->size;
    res->array = (size_t *)calloc(res->length, sizeof(size_t));
    for (size_t i = 0; i < res->length; i++)
    {
        coord_t *fuck = pop(dupa);
        if (*(fuck->coord) == (size_t)0)
        {
            printf("ebe\n");
            fwrite("ERROR 9\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
        else
        {
            res->array[i] = *(fuck->coord);
            free(fuck->coord);
            free(fuck);
        }
    }
    queue_destroy(dupa);
    return res;
}

int main()
{
    char *line = NULL;
    size_t len = 0;
    size_t read;
    if ((fseek(stdin, 0, SEEK_END), ftell(stdin)) > 0)
    {
        rewind(stdin);
        // wczytaj wymiar labiryntu
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            fwrite("ERROR 1\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
        array_with_length_t *dimentions = line_to_array(line);
        // wczytaj wejście do labiryntu
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            fwrite("ERROR 2\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
        array_with_length_t *entry = line_to_array(line);
        if (entry->length != dimentions->length)
        {
            fwrite("ERROR 2\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
        // wczytaj wyjście z labiryntu
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            fwrite("ERROR 3\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
        array_with_length_t *ext = line_to_array(line);
        if (ext->length != entry->length)
        {
            fwrite("ERROR 3\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
        for (size_t i = 0; i < dimentions->length; i++)
        {
            printf("%zu\n", dimentions->array[i]);
            printf("%zu\n", entry->array[i]);
            printf("%zu\n", ext->array[i]);
        }
        free(ext->array);
        free(ext);
        free(dimentions->array);
        free(dimentions);
        free(entry->array);
        free(entry);
        // wczytywanie czwartej linii
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            fwrite("ERROR 4\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
        const char s[2] = " ";
        char *token;
        token = strtok(line, s);
        if (token[0] == 'R')
        {
            parce_R(token);
        }
        else if (token[0] == '0')
        {
            if (token++ != NULL)
            {
                if (token[0] == 'x')
                {
                    token++;
                    parce_hex(token);
                }
                else
                {
                    printf("chuj\n");
                    fwrite("ERROR 4\n", 8, 1, stderr);
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                fwrite("ERROR 4\n", 8, 1, stderr);
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            fwrite("ERROR 4\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
        read = getline(&line, &len, stdin);
        if((int)read != -1){
            fwrite("ERROR 5\n", 8, 1, stderr);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fwrite("ERROR 1\n", 8, 1, stderr);
        exit(EXIT_FAILURE);
    }
    free(line);
    return 1;
}
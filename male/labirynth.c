#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "queue.h"
#include "bitarray.h"
#include "types.h"
#include "parsing.h"

void fail(int i)
{
    fprintf(stderr, "ERROR %i\n", i);
    exit(EXIT_FAILURE);
}

void free_array(array_with_length_t *obj)
{
    free(obj->array);
    free(obj);
}

bool compare_coords(array_with_length_t a, array_with_length_t b)
{
    bool res = true;
    for (size_t i = 0; i < a.length; i++)
    {
        if ((a.array)[i] != (b.array)[i])
        {
            res = false;
            break;
        }
    }
    return res;
}

queue_t *add_neighbours(queue_t *queue, arr_t *wall, arr_t *visited, array_with_length_t *coord, size_t dist)
{
    for (size_t i = 0; i < coord->length; i++)
    {

        for (int t = -1; t <= 1; t = t + 2)
        {
            size_t *new = (size_t *)calloc(coord->length, sizeof(size_t));
            if (!new)
            {
                return NULL;
            }
            for (size_t j = 0; j < coord->length; j++)
            {
                new[j] = coord->array[j];
            }
            new[i] = new[i] + t;
            coord_t *nelem = (coord_t *)malloc(sizeof(coord_t));
            if (!nelem)
            {
                free(new);
                return NULL;
            }

            nelem->len = dist;
            nelem->coord.length = coord->length;
            nelem->coord.array = new;
            if (new[i] <= visited->settings->dimmaxes[i] && new[i] > 0)
            {
                signed char is_wall = get(wall, new);
                signed char is_visited = get(visited, new);
                if (is_wall == -1 || is_visited == -1)
                {
                    free(new);
                    free(nelem);
                    return NULL;
                }
                if (!is_wall && !is_visited)
                {
                    queue = add(queue, nelem);
                }
                else
                {
                    free(nelem->coord.array);
                    free(nelem);
                }
            }
            else
            {
                free(nelem->coord.array);
                free(nelem);
            }
        }
    }
    return queue;
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
            free(line);
            fail(1);
        }
        bool memfail = false;
        array_with_length_t *dimentions = line_to_array(line, &memfail);
        if (!dimentions)
        {
            free(line);
            if (memfail)
            {
                fail(0);
            }
            else
            {
                fail(1);
            }
        }
        arr_t *visited = initialize_array(dimentions);
        if (!visited)
        {
            free(line);
            fail(0);
        }
        // wczytaj wejście do labiryntu
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            free(line);
            free_array(dimentions);
            destroy_array(visited);
            fail(2);
        }
        memfail = false;
        array_with_length_t *entry = line_to_array(line, &memfail);
        if (!entry)
        {
            free(line);
            free_array(dimentions);
            destroy_array(visited);
            if (memfail)
            {
                fail(0);
            }
            else
            {
                fail(1);
            }
        }
        if (entry->length != dimentions->length)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            destroy_array(visited);
            fail(2);
        }
        // wczytaj wyjście z labiryntu
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            destroy_array(visited);
            fail(3);
        }
        memfail = false;
        array_with_length_t *ext = line_to_array(line, &memfail);
        if (!ext)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            destroy_array(visited);
            if (memfail)
            {
                fail(0);
            }
            else
            {
                fail(3);
            }
        }
        if (ext->length != entry->length)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            destroy_array(visited);
            fail(3);
        }
        //  wczytywanie ścian
        arr_t *walls;
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            destroy_array(visited);
            fail(4);
        }
        const char s[2] = " ";
        char *token;
        token = strtok(line, s);
        if (token[0] == 'R')
        {
            memfail = false;
            walls = parce_R(token, dimentions, &memfail);
            if (!walls)
            {
                free(line);
                free_array(entry);
                free_array(dimentions);
                free_array(ext);
                destroy_array(visited);
                if (memfail)
                {
                    fail(0);
                }
                else
                {
                    fail(4);
                }
            }
        }
        else if (token[0] == '0')
        {
            if (token + 1 != NULL)
            {
                if (token[1] == 'x')
                {
                    memfail = false;
                    walls = parce_hex(token, dimentions, &memfail);
                    if (!walls)
                    {
                        free(line);
                        free_array(entry);
                        free_array(dimentions);
                        free_array(ext);
                        destroy_array(visited);
                        if (memfail)
                        {
                            fail(0);
                        }
                        else
                        {
                            fail(4);
                        }
                    }
                }
                else
                {
                    free(line);
                    free_array(entry);
                    free_array(dimentions);
                    free_array(ext);
                    destroy_array(visited);
                    fail(4);
                }
            }
            else
            {
                free(line);
                free_array(entry);
                free_array(dimentions);
                free_array(ext);
                destroy_array(visited);
                fail(4);
            }
        }
        else
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            destroy_array(visited);
            fail(4);
        }
        read = getline(&line, &len, stdin);
        if ((int)read != -1)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            destroy_array(visited);
            destroy_array(walls);
            fail(5);
        }

        // jeśli program dotrze tu to znaczy że dane są poprawnie wpisane. teraz pora na zaaplikowanie bfsa
        queue_t *queue = queue_initialize(10);
        if (!queue)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            destroy_array(visited);
            destroy_array(walls);
            fail(0);
        }
        signed char is_wall = get(walls, entry->array);
        if (is_wall == -1)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            destroy_array(visited);
            destroy_array(walls);
            fail(0);
        }
        if (!is_wall)
        {
            coord_t *initial = (coord_t *)malloc(sizeof(coord_t));
            if (!initial)
            {
                free(line);
                free_array(entry);
                free_array(dimentions);
                free_array(ext);
                destroy_array(visited);
                destroy_array(walls);
                fail(0);
            }
            initial->len = 0;
            initial->coord = *(entry);
            queue = add(queue, initial);
            if (!queue)
            {
                free(line);
                free_array(dimentions);
                free_array(ext);
                destroy_array(visited);
                destroy_array(walls);
                free(initial);
                free(entry);
                fail(0);
            }
        }
        else
        {
            free(entry->array);
        }
        bool done = false;
        while (!is_empty(queue))
        {

            coord_t *suspect = pop(queue);
            if (compare_coords(suspect->coord, *ext))
            {
                fprintf(stdout, "%zu\n", suspect->len);
                done = true;
            }
            else
            {
                put(visited, suspect->coord.array);
                queue = add_neighbours(queue, walls, visited, &suspect->coord, suspect->len + 1);

                if (!queue)
                {
                    queue_destroy(queue);
                    free(line);
                    free_array(entry);
                    free_array(dimentions);
                    free_array(ext);
                    destroy_array(visited);
                    destroy_array(walls);
                    free(suspect->coord.array);
                    free(suspect);
                    fail(0);
                }
            }
            free(suspect->coord.array);
            free(suspect);
            if (done)
            {
                break;
            }
        }
        if (!done)
        {
            fprintf(stdout, "NO WAY\n");
        }
        queue_destroy(queue);
        free_array(ext);
        free_array(dimentions);
        free(entry);
        destroy_array(visited);
        destroy_array(walls);
    }
    else
    {
        free(line);
        fail(1);
    }
    free(line);
    return 1;
}
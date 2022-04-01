#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include "queue.h"
#include "bitarray.h"

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
        // printf("%zu\n",(a.array)[i]);
        if ((a.array)[i] != (b.array)[i])
        {
            res = false;
            // break;
        }
    }
    return res;
}

size_t get_labirynth_size(array_with_length_t *dimentions)
{
    size_t res = 1;
    for (size_t i = 0; i < dimentions->length; i++)
    {
        if (SIZE_MAX / dimentions->array[i] > res)
        {
            res = res * dimentions->array[i];
        }
        else
        {
            fail(0);
        }
    }
    return res;
}

arr_t *parce_R(char *token, array_with_length_t *dimentions)
{
    const char sep[2] = " ";
    long unsigned int res[5];
    int i;
    if (token++ != NULL)
    {
        sscanf(token, "%lu", &res[0]);
        i = 1;
    }
    else
    {
        i = 0;
    }
    token = strtok(NULL, sep);
    while (token != NULL)
    {
        sscanf(token, "%lu", &res[i]);
        i++;
        token = strtok(NULL, sep);
    }
    if (i < 5)
    {
        fail(4);
    }
    long unsigned int* w = (long unsigned int*)calloc(res[3],sizeof(long unsigned int));
    if(w == NULL)
    {
        fail(0);
    }
    //printf("tbh it worked longer than expected\n");
    w[0] = res[4];
    for (long unsigned int i = 1; i <= res[3]; i++)
    {
        w[i] = (res[0] * w[i - 1] + res[1]) % res[2];
    }
    size_t size = get_labirynth_size(dimentions);
    for (long unsigned int i = 0; i <= res[3]; i++)
    {
        w[i] = w[i] % size;
    }
    arr_t *walls = initialize_array(dimentions);
    long unsigned int temp;
    for (size_t i = 0; i < size; i++)
    {
        temp = (long unsigned int)(i % ((size_t)1 << 32));
        bool filled = false;
        for (long unsigned int j = 0; j <= res[3]; j++)
        {
            if (w[j] == temp)
            {
                filled = true;
                break;
            }
        }
        if (filled)
        {
            indx_t index;
            index.cell = (i) / (walls->settings->mx);
            index.rem = (i) % (walls->settings->mx);

            put_in_array(walls, &index);
        }
    }
    return walls;
}

void parce_hex(char *token)
{
    size_t number = strtoul(token, NULL, 16);
    printf("%zu\n", number);
}

array_with_length_t *line_to_array(char *str, int line_number)
{
    const char s[2] = " ";
    char *token;
    token = strtok(str, s);
    queue_t *dupa = queue_initialize(1);
    while (token != NULL)
    {
        size_t len = 0;
        sscanf(token, "%zu", &len);
        coord_t *a = (coord_t *)malloc(sizeof(coord_t));
        a->coord.length = 1;
        a->coord.array = (size_t *)malloc(sizeof(size_t));
        *(a->coord.array) = len;
        dupa = add(dupa, a);
        token = strtok(NULL, s);
    }
    array_with_length_t *res = (array_with_length_t *)malloc(sizeof(array_with_length_t));
    res->length = dupa->size;
    res->array = (size_t *)calloc(res->length, sizeof(size_t));
    for (size_t i = 0; i < res->length; i++)
    {
        coord_t *fuck = pop(dupa);
        if (*(fuck->coord.array) == (size_t)0)
        {
            fail(line_number);
        }
        else
        {
            res->array[i] = *(fuck->coord.array);
            free(fuck->coord.array);
            free(fuck);
        }
    }
    queue_destroy(dupa);
    return res;
}

queue_t *add_neighbours(queue_t *queue, arr_t *wall, arr_t *visited, array_with_length_t *coord, size_t dist)
{
    // for (size_t j = 0; j < coord->length; j++)
    // {
    //     printf("%zu ", coord->array[j]);
    // }
    // printf("\n\n");
    for (size_t i = 0; i < coord->length; i++)
    {
        // printf("begin\n");
        size_t *new = (size_t *)calloc(coord->length, sizeof(size_t));
        for (size_t j = 0; j < coord->length; j++)
        {
            new[j] = coord->array[j];
        }
        new[i]++;
        // for (size_t j = 0; j < coord->length; j++)
        // {
        //     printf("%zu ", new[j]);
        // }
        // printf("\n");
        coord_t *nelem = (coord_t *)malloc(sizeof(coord_t));
        nelem->len = dist;
        nelem->coord.length = coord->length;
        nelem->coord.array = new;
        if (new[i] <= visited->settings->dimmaxes[i] && !get(wall, new) && !get(visited, new))
        {
            queue = add(queue, nelem);
        }
        else
        {
            free(nelem->coord.array);
            free(nelem);
        }
        // wtf
        size_t *new2 = (size_t *)calloc(coord->length, sizeof(size_t));
        for (size_t j = 0; j < coord->length; j++)
        {
            new2[j] = coord->array[j];
        }
        new2[i]--;
        // for (size_t j = 0; j < coord->length; j++)
        // {
        //     printf("%zu ", new2[j]);
        // }
        // printf("\n");
        coord_t *nelem2 = (coord_t *)malloc(sizeof(coord_t));
        nelem2->len = dist;
        nelem2->coord.length = coord->length;
        nelem2->coord.array = new2;
        if (new2[i] > 0 && !get(wall, new2) && !get(visited, new2))
        {
            queue = add(queue, nelem2);
        }
        else
        {
            free(nelem2->coord.array);
            free(nelem2);
        }
        // printf("end\n");
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
        array_with_length_t *dimentions = line_to_array(line, 1);
        arr_t *visited = initialize_array(dimentions);

        // wczytaj wejście do labiryntu
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            free(line);
            free_array(dimentions);
            fail(2);
        }
        array_with_length_t *entry = line_to_array(line, 2);
        if (entry->length != dimentions->length)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            fail(2);
        }
        // wczytaj wyjście z labiryntu
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            fail(3);
        }
        array_with_length_t *ext = line_to_array(line, 3);
        if (ext->length != entry->length)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            fail(3);
        }
        //  wczytywanie ścian
        arr_t *walls;
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            free(line);
            fail(4);
        }
        const char s[2] = " ";
        char *token;
        token = strtok(line, s);
        if (token[0] == 'R')
        {
            walls = parce_R(token, dimentions);
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
                    free(line);
                    fail(4);
                }
            }
            else
            {
                free(line);
                fail(4);
            }
        }
        else
        {
            free(line);
            fail(4);
        }
        read = getline(&line, &len, stdin);
        if ((int)read != -1)
        {
            free(line);
            fail(5);
        }
        queue_t *queue = queue_initialize(7); // zmienić to kurwa potem
        if (!get(walls, entry->array))
        {
            coord_t *initial = (coord_t *)malloc(sizeof(coord_t));
            initial->len = 0;
            initial->coord = *(entry);
            queue = add(queue, initial);
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
                printf("oh fuck\n");
                printf("%zu\n", suspect->len);
                done = true;
            }
            else
            {
                put(visited, suspect->coord.array);
                queue = add_neighbours(queue, walls, visited, &suspect->coord, suspect->len + 1);
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
            printf("NO WAY\n");
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
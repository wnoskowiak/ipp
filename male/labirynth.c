#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>
#include "queue.h"
#include "bitarray.h"

static void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

typedef struct indx
{
    size_t cell;
    size_t rem;
} indx_t;

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
    printf("here\n");
    bool res = true;
    for(int i =0; i<a.length; i++){
        printf("here\n");
        if((a.array)[i]!= (b.array)[i])
        {
            res = false;
            break;
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

arr_t * parce_R(char *token, array_with_length_t *dimentions)
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
    long unsigned int w[res[3]];
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
            index.cell = (i)/(walls->settings->mx);
            index.rem = (i)%(walls->settings->mx);

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
//playground
size_t* aa = (size_t*)calloc(3,sizeof(size_t));
aa[0] = 2;
aa[1] = 2;
aa[2] = 2;
size_t* bb = (size_t*)calloc(3,sizeof(size_t));
bb[0] = 2;
bb[1] = 2;
bb[2] = 2;
array_with_length_t a,b;
a.length = 3;
b.length = 3;
a.array = aa;
a.array = bb;
printf(compare_coords(a,b)? "true\n" : "false\n");

//


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
        array_with_length_t *dimentions = line_to_array(line);
        arr_t *visited = initialize_array(dimentions);
        destroy_array(visited);
        // wczytaj wejście do labiryntu
        read = getline(&line, &len, stdin);
        if ((int)read == -1)
        {
            free(line);
            free_array(dimentions);
            fail(2);
        }
        array_with_length_t *entry = line_to_array(line);
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
        array_with_length_t *ext = line_to_array(line);
        if (ext->length != entry->length)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            fail(3);
        }
        free_array(entry);
        free_array(ext);
        //  wczytywanie ścian
        arr_t* walls;
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
        free_array(dimentions);
        read = getline(&line, &len, stdin);
        if ((int)read != -1)
        {
            free(line);
            fail(5);
        }
    }
    else
    {
        free(line);
        fail(1);
    }
    free(line);
    return 1;
}
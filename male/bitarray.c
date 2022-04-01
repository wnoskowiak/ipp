#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bitarray.h"

/*
zwykły bfs działa, ale trzeba nie spierdolić
labirynt można trzymać jako tablice intów
*/

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

arr_t *initialize_array(array_with_length_t* dimention_definitions)
{
	arr_t *res = (arr_t *)malloc(sizeof(arr_t));
	if (!res)
	{
		return NULL;
	}
	res->settings = (settings_t *)malloc(sizeof(settings_t));
	if (!res->settings)
	{
		free(res);
		return NULL;
	}
	res->settings->dimmaxes = dimention_definitions->array;
	//printf("%zu\n",res->settings->dimmaxes[0]);
	res->settings->dimnum = dimention_definitions->length;
	res->settings->mx = (size_t)((log((SIZE_MAX >> 1) + 1) / log(2)) + 1);
	size_t temp = 1;
	for (size_t i = 0; i < (res->settings->dimnum); i++)
	{
		temp = temp * ((res->settings->dimmaxes)[i]);
	}
	temp++;
	temp = ((size_t)(temp / res->settings->mx) + 1);
	res->array = (size_t *)calloc(temp, sizeof(size_t));
	if (!res->array)
	{
		free(res->settings);
		free(res);
		return NULL;
	}
	return res;
}

void destroy_array(arr_t *array){
	free(array->array);
	free(array->settings);
	free(array);
}

indx_t *to_internal_type(arr_t *array, size_t *coords)
{
	//printf("kurwaaaaaa\n");
	size_t pom = 0;
	for (size_t i = array->settings->dimnum - 1; i > 0; i--)
	{
		//printf("%zu\n", i);
		if ((coords[i]) > array->settings->dimmaxes[i])
		{
			return NULL;
		}
		//printf("jak ty to kuwa tak zjebałeś?\n");
		pom = (pom + coords[i] -1) * array->settings->dimmaxes[i - 1];
	}
	pom = pom + coords[0] - 1;
	//printf("%zu\n", pom);
	indx_t *res = (indx_t *)malloc(sizeof(indx_t));
	res->cell = pom / array->settings->mx;
	res->rem = pom % array->settings->mx;
	return res;
}

void put_in_array(arr_t *array, indx_t *index)
{
	array->array[index->cell] = array->array[index->cell] | ((size_t)1 << (index->rem ));
}

bool put(arr_t *array, size_t *coords)
{
	//printf("here we go again\n");
	indx_t *tmp = to_internal_type(array, coords);
	//printf("fuck my fucking life\n");
	if (!tmp)
	{
		return false;
	}
	put_in_array(array, tmp);
	free(tmp);
	return true;
}

bool get_from_array(arr_t *array, indx_t *index)
{
	// printBits(sizeof(array->array[index->cell]),&array->array[index->cell]);
	size_t sranie = ((size_t)1 << (index->rem));
	// printBits(sizeof(sranie),&sranie);
	return((array->array[index->cell] & ((size_t)1 << (index->rem))));
}

signed char get(arr_t *array, size_t *coords)
{
	indx_t *tmp = to_internal_type(array, coords);
	if (!tmp)
	{
		return -1;
	}
	bool bol_value = get_from_array(array, tmp);
	free(tmp);
	return (signed char)bol_value;
}

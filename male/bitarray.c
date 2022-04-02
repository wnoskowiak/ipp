#include <stdio.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bitarray.h"
#include "types.h"

/* implementacja tablic binarnych */

//inicjalizacja tablicy
arr_t *initialize_array(array_with_length_t* dimention_definitions)
{
	//alokacja pamięci oraz ustawianie struktury przechowywującej ustawienia tablicy
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
	res->settings->dimnum = dimention_definitions->length;
	//obliczanie ilości bitów w size max
	res->settings->mx = (size_t)((log((SIZE_MAX >> 1) + 1) / log(2)) + 1);
	//określanie potrzebnej ilośći komórek o mx bitach do przechowania informacji o wszystkich kostkach labiryntu
	size_t temp = 1;
	for (size_t i = 0; i < (res->settings->dimnum); i++)
	{
		temp = temp * ((res->settings->dimmaxes)[i]);
	}
	temp++;
	temp = ((size_t)(temp / res->settings->mx) + 1);
	//alokacja tablicy
	res->array = (size_t *)calloc(temp, sizeof(size_t));
	if (!res->array)
	{
		free(res->settings);
		free(res);
		return NULL;
	}
	return res;
}

//funkcja zwalniająca pamięć zajmowaną przez tablicę
void destroy_array(arr_t *array){
	free(array->array);
	free(array->settings);
	free(array);
}

//funkcja przepisująca współrzędne w kostce na dane w formacie: cell - komórka w tabeli; res -> bit w tej komórce
indx_t *to_internal_type(arr_t *array, size_t *coords)
{
	//obliczanie mapowania na bit
	size_t pom = 0;
	for (size_t i = array->settings->dimnum - 1; i > 0; i--)
	{
		if ((coords[i]) > array->settings->dimmaxes[i])
		{
			return NULL;
		}
		pom = (pom + coords[i] -1) * array->settings->dimmaxes[i - 1];
	}
	pom = pom + coords[0] - 1;
	//przetwarzanie na wcześniej określony typ danych 
	indx_t *res = (indx_t *)malloc(sizeof(indx_t));
	res->cell = pom / array->settings->mx;
	res->rem = pom % array->settings->mx;
	return res;
}

//funkcja pomocnicza ustawiająca watrość bitu w tablicy na podstawie inputu w formacie {cell - komórka w tabeli; res -> bit w tej komórce} na 1
void put_in_array(arr_t *array, indx_t *index)
{
	array->array[index->cell] = array->array[index->cell] | ((size_t)1 << (index->rem ));
}

//funkcja ustawiająca wartość kostki na którą wskazują podane współrzędne na 1
bool put(arr_t *array, size_t *coords)
{
	// przetwarzanie koordyntów na wyżej opisany typ danych 
	indx_t *tmp = to_internal_type(array, coords);
	if (!tmp)
	{
		return false;
	}
	// zapis do tabeli
	put_in_array(array, tmp);
	free(tmp);
	return true;
}

//funkcja pomocnicza odczytująca watrość bitu w tablicy na podstawie inputu w formacie {cell - komórka w tabeli; res -> bit w tej komórce}
static bool get_from_array(arr_t *array, indx_t *index)
{
	return((array->array[index->cell] & ((size_t)1 << (index->rem))));
}

//funkcja odczytująca wartość kostki na którą wskazują podane współrzędne
signed char get(arr_t *array, size_t *coords)
{
	// przetwarzanie koordyntów na wyżej opisany typ danych
	indx_t *tmp = to_internal_type(array, coords);
	if (!tmp)
	{
		return -1;
	}
	// odczyt z tabeli
	bool bol_value = get_from_array(array, tmp);
	free(tmp);
	return (signed char)bol_value;
}

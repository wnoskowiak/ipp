#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include "queue.h"
#include "bitarray.h"
#include "types.h"

/* implementacja funkcji odpowiedzialnych za parsowanie danych wejściowych */

//funkcja określająca objętość labiryntu, czyli wymnażająca jego wymiary 
static size_t *get_labirynth_size(array_with_length_t *dimentions)
{
    size_t *res = (size_t *)malloc(sizeof(size_t));
    if (!res)
    {
        return NULL;
    }
    *res = 1;
    for (size_t i = 0; i < dimentions->length; i++)
    {
        if (SIZE_MAX / dimentions->array[i] > *res)
        {
            *res = *res * dimentions->array[i];
        }
        else
        {
            free(res);
            return NULL;
        }
    }
    return res;
}

//funkcja określająca położenie ścian labiryntu na podstawie serii liczb zaczynających się od R
arr_t *parce_R(char *token, array_with_length_t *dimentions, bool *memfail)
{
    long unsigned int res[5];
    int i;
    //sprawdzam czy po R występowały jakieś znaki niedrukowane
    if (token + 1 != NULL)
    {
        if (token[1] != '\0')
        {
            //jeśli nie to zakładam że słowo zaczyna się od R
            sscanf(token, "R%lu", &res[0]);
            i = 1;
        }
        else
        {
            i = 0;
        }
    }
    //jeśli tak to przechodzę do następnego słowa
    else
    {
        i = 0;
    }
    //pobieranie liczby z linii
    token = strtok(NULL, " \n\t");
    while (token != NULL)
    {
        //parseowanie liczby do natywnych typów
        sscanf(token, "%lu", &res[i]);
        i++;
        token = strtok(NULL, " \n\t");
    }
    if (i < 5)
    {
        //sprawdzam czy podano odpowiednią ilość liczb
        return NULL;
    }
    //wyliczanie wartośći s
    long unsigned int *w = (long unsigned int *)calloc(res[3] + 1, sizeof(long unsigned int));
    if (!w)
    {
        *memfail = true;
        return NULL;
    }
    w[0] = res[4];
    for (long unsigned int i = 1; i <= res[3]; i++)
    {
        w[i] = (res[0] * w[i - 1] + res[1]) % res[2];
    }
    size_t *size = get_labirynth_size(dimentions);
    if (!size)
    {
        free(w);
        *memfail = true;
        return NULL;
    }
    //wyliczanie wartości w
    for (long unsigned int i = 0; i <= res[3]; i++)
    {
        w[i] = w[i] % *size;
    }
    //generowanie tabeli reprezentującej ściany labiryntu
    arr_t *walls = initialize_array(dimentions);
    if (!walls)
    {
        *memfail = true;
        free(size);
        free(w);
        return NULL;
    }
    long unsigned int temp;
    for (size_t i = 0; i < *size; i++)
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
    free(size);
    free(w);
    return walls;
}

//funkcja określająca położenie ścian labiryntu na podstawie liczby szesnastkowej
arr_t *parce_hex(char *token, array_with_length_t *dimentions, bool *memfail)
{
    errno = 0;
    char *temp = token;
    //przepisywanie liczby na natywne typy
    unsigned long long number = strtoull(token, NULL, 0);
    if (errno != 0)
    {
        return NULL;
    }
    //sprawdzanie czy podana licba nie jest ujemna
    else if (number == 0)
    {
        unsigned long long safety = strtoull(&temp[2], NULL, 16);
        if (safety != 0)
        {
            return NULL;
        }
    }
    size_t *size = get_labirynth_size(dimentions);
    if (!size)
    {
        *memfail = true;
        return NULL;
    }
    size_t bit_num = (size_t)((log((ULLONG_MAX >> 1) + 1) / log(2)) + 1);
    //inicjalizacja tablicy reprezentującej ściany labiryntu i wypełnianie danych
    arr_t *walls = initialize_array(dimentions);
    if (!walls)
    {
        free(size);
        *memfail = true;
        return NULL;
    }
    for (size_t i = 0; i < bit_num && i < *size && number > 0; i++)
    {
        if (number % 2 == 1)
        {
            indx_t index;
            index.cell = (i) / (walls->settings->mx);
            index.rem = (i) % (walls->settings->mx);
            put_in_array(walls, &index);
        }
        number = number / 2;
    }
    free(size);
    return walls;
}

//parseowanie danych o współrzędnych 
array_with_length_t *line_to_array(char *str, bool *memfail)
{
    //inicjalizacja kolejki i początek odczytu danych
    char *token;
    token = strtok(str, " \n\t");
    queue_t *queue = queue_initialize(1);
    if (!queue)
    {
        return NULL;
    }
    //zbieramy liczy do końca liniii
    while (token != NULL)
    {
        //odczyt liczby z linii i przetworzenie na natwne typy danych 
        errno = 0;
        size_t len = strtoul(token, NULL, 10);
        //sprawdzanie czy liczby zostały odczytane poprawnie
        if(errno != 0)
        {
            queue_destroy(queue);
            return NULL;
        }
        //dodawanie odczytanej liczby do kolejki
        coord_t *a = (coord_t *)malloc(sizeof(coord_t));
        if (!a)
        {
            queue_destroy(queue);
            *memfail = true;
            return NULL;
        }
        a->coord.length = 1;
        a->coord.array = (size_t *)malloc(sizeof(size_t));
        if (!a->coord.array)
        {
            queue_destroy(queue);
            free(a);
            *memfail = true;
            return NULL;
        }
        *(a->coord.array) = len;
        queue = add(queue, a);
        token = strtok(NULL, " \n\t");
    }
    //tworzenie struktury reprezentującej współrzędne w labiryncie 
    array_with_length_t *res = (array_with_length_t *)malloc(sizeof(array_with_length_t));
    if (!res)
    {
        queue_destroy(queue);
        *memfail = true;
        return NULL;
    }
    //określanie wymiaru labiryntu na podstawie długości kolejki
    res->length = queue->size;
    res->array = (size_t *)calloc(res->length, sizeof(size_t));
    if (!res->array)
    {
        queue_destroy(queue);
        free(res);
        *memfail = true;
        return NULL;
    }
    //wypełnianie struktury danymi
    for (size_t i = 0; i < res->length; i++)
    {
        coord_t *element = pop(queue);
        if (!element)
        {
            queue_destroy(queue);
            free(res->array);
            free(res);
            return NULL;
        }
        if (*(element->coord.array) == (size_t)0)
        {
            queue_destroy(queue);
            free(element->coord.array);
            free(element);
            free(res->array);
            free(res);
            return NULL;
        }
        else
        {
            res->array[i] = *(element->coord.array);
            free(element->coord.array);
            free(element);
        }
    }
    queue_destroy(queue);
    return res;
}

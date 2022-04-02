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

/* główny plik odpowiedzialny za obsługę błędów i przeszukiwanie labiryntu */

// funkcja wywoływana w celu wyrzucenia błędu
void fail(int i)
{
    fprintf(stderr, "ERROR %i\n", i);
    exit(EXIT_FAILURE);
}

// zwalnianie pamięci struktury array
void free_array(array_with_length_t *obj)
{
    free(obj->array);
    free(obj);
}

// porównywanie dwóch współrzędnych kostek
bool compare_coords(array_with_length_t a, array_with_length_t b)
{
    // współrzędne są takie same wtedy i tylko wtedy gdy ich wszystkie komponenty są sobie równe
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

// dodawanie sąsiadów kostki do kolejki
queue_t *add_neighbours(queue_t *queue, arr_t *wall, arr_t *visited, array_with_length_t *coord, size_t dist)
{
    // przejście przez wszyzstkie wymiary labiryntu
    for (size_t i = 0; i < coord->length; i++)
    {
        // próba zwiększenia i zmniejszenia wymiaru
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
            // sprawdzenie czy współrzędna nie wyszchodzi poza labirynt
            if (new[i] <= visited->settings->dimmaxes[i] && new[i] > 0)
            {
                // sprawdzenie czy współrzędna nie została już odwiedzona lub czy nie jest ścianą
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
                    // jeśli współrzędna spełnia warunki jest dodawana do kolejki
                    queue = add(queue, nelem);
                }
                // jeśli nie funkcja oddaje zarezerwowaną pamięć
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
    // sprawdzanie czy wejście nie jest puste
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
        // w przypadku błędu zwracana jest pamięć
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
        // tworzenie listy odwiedzonych kostek
        arr_t *visited = initialize_array(dimentions);
        // w przypadku błędu zwracana jest pamięć
        if (!visited)
        {
            free(line);
            fail(0);
        }
        // wczytaj wejście do labiryntu
        // czytanie linni z wejścia
        read = getline(&line, &len, stdin);
        // w przypadku błędu zwracana jest pamięć
        if ((int)read == -1)
        {
            free(line);
            free_array(dimentions);
            destroy_array(visited);
            fail(2);
        }
        memfail = false;
        array_with_length_t *entry = line_to_array(line, &memfail);
        // w przypadku błędu zwracana jest pamięć
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
        // jeśli wczytane dane mają różną ilość współrzędnych to muszą być niepoprawne
        if (entry->length != dimentions->length)
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            destroy_array(visited);
            fail(2);
        }
        // wczytaj wyjście z labiryntu
        // czytanie linni z wejścia
        read = getline(&line, &len, stdin);
        // w przypadku błędu zwracana jest pamięć
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
        // w przypadku błędu zwracana jest pamięć
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
        // znowu, jeśli zebrane dane są różnych wymiarów to na pewno są błędne
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
        // tworzenie listy reprezentującej ściany labiryntu
        arr_t *walls;
        // czytanie linni z wejścia
        read = getline(&line, &len, stdin);
        // w przypadku błędu zwracana jest pamięć
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
        // określanie formatowania informacji o ścianach labiryntu
        if (token[0] == 'R')
        {
            memfail = false;
            // parecowanie danych zaczynających się od R
            walls = parce_R(token, dimentions, &memfail);
            // w przypadku błędu zwracana jest pamięć
            if (!walls)
            {
                if (memfail)
                {
                    fail(0);
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
        }
        else if (token[0] == '0')
        {
            if (token + 1 != NULL)
            {
                if (token[1] == 'x')
                {
                    memfail = false;
                    // parceowanie danch w formacie liczby szesnastkowej
                    walls = parce_hex(token, dimentions, &memfail);
                    // w przypadku błędu zwalniana jest pamięć
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
                // obsługa błędów
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
        // każdy inny tekst musi być błędny
        else
        {
            free(line);
            free_array(entry);
            free_array(dimentions);
            free_array(ext);
            destroy_array(visited);
            fail(4);
        }
        // jeśli da się odczytać kolejną linijkę to znaczy że dane muszą być błędne
        read = getline(&line, &len, stdin);
        // w przypadku błędu zwalniana jest pamięć
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
        // jeśli program dotrze tu to znaczy że dane są poprawnie wpisane. od tąd zaczyna się implementacja bfsa
        // inicjalizacja kolejki
        queue_t *queue = queue_initialize(visited->settings->dimnum);
        // w przypadku błędu alokacji
        if (!queue)
        {
            fail(0);
        }
        // sprawdzenie czy wejście do labiryntu nie jest w pełnej kostce
        signed char is_wall = get(walls, entry->array);
        // w przypadku błędu alokacji
        if (is_wall == -1)
        {
            fail(0);
        }
        if (!is_wall)
        {
            // jeśli wejście do labiryntu nie jest scianą dodawane jest ono do kolejki
            coord_t *initial = (coord_t *)malloc(sizeof(coord_t));
            // w przypadku błędu alokacji
            if (!initial)
            {
                fail(0);
            }
            initial->len = 0;
            initial->coord = *(entry);
            queue = add(queue, initial);
            // w przypadku błędu alokacji
            if (!queue)
            {
                fail(0);
            }
        }
        // jeśli wejście do labiryntu jest scianą to zwracany jest error 2
        else
        {
            free(line);
            free_array(dimentions);
            free_array(ext);
            destroy_array(visited);
            destroy_array(walls);
            free_array(entry);
            queue_destroy(queue);
            fail(2);
        }
        // zmienna określająca czy znaleziono rozwiązanie
        bool done = false;
        // droga szukana jest najpóźniej do czasu opróżnienia kolejki
        while (!is_empty(queue))
        {
            // pobieranie pierwszego elementu kolejki
            coord_t *suspect = pop(queue);
            // w przypadku błędu alokacji
            if (!suspect)
            {
                fail(0);
            }
            // sprawdzanie czy jest on wyjściem z labiryntu
            if (compare_coords(suspect->coord, *ext))
            {
                // jeśli jest wypisuje długosć drogi jaka do niego prowadziła i kończę szukanie
                fprintf(stdout, "%zu\n", suspect->len);
                done = true;
            }
            else
            {
                // jeśli nie jest to aktualna pozycja dodawana jest do odwiedzonych, a jej sasiedzi dodawani do kolejki
                put(visited, suspect->coord.array);
                queue = add_neighbours(queue, walls, visited, &suspect->coord, suspect->len + 1);
                // w przypadku błędu alokacji
                if (!queue)
                {
                    fail(0);
                }
            }
            // zwalnianie pamięci przeznaczonej na element z kolejki
            free(suspect->coord.array);
            free(suspect);
            // przerywanie pętli w przypadku znalezienia rozwiązania
            if (done)
            {
                break;
            }
        }
        // jeśli nie da się znaleźć rozwiązania wypisuję NO WAY
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
    // zwalnianie pamięci
    else
    {
        free(line);
        fail(1);
    }
    free(line);
    return 1;
}
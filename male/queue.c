#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "queue.h"
#include "types.h"


/*implementacja kolejki przechowującej wskaźniki na współrzędne elementów labiryntu*/


//inicjalizacja kolejki 
queue_t* queue_initialize(size_t cap)
{
    queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
    queue->cap = cap;
    queue->front = queue->size = 0;
    queue->back = cap - 1;
    queue->array = (coord_t **)calloc(cap, sizeof(coord_t *));
    if(queue->array == NULL)
    {
        return NULL;
    }
    return queue;
}

//zwalnianie pamięci kolejki
void queue_destroy(queue_t *queue)
{
    coord_t* temp;
    while (!is_empty(queue))
    {
        temp = pop(queue);
        free(temp->coord.array);
        free(temp);
    }
    free(queue->array);
    free(queue);
}

//zwiększanie rozmiaru kolejki
queue_t* queue_resize(queue_t* queue)
{
    coord_t **narr = (coord_t **)realloc(queue->array, (queue->cap + 2) * sizeof(coord_t *));
    if (narr == NULL)
    {
        queue_destroy(queue);
        return NULL;
    }
    queue->cap += 2;
    queue->array = narr;
    //funkcja realloc dodaje pusty wskaźnik na końcu struktury, należy upewnić się że nie zawiera się on w dancyh przechowywanych w kolejce
    if (queue->back < queue->front)
    {
        for (size_t i = 0; i < queue->back + 1; i++)
        {
            queue->array[(queue->cap - 2 + i) % queue->cap] = queue->array[i];
        }
        queue->back = (queue->cap - 2 + queue->back) % queue->cap;
    }
    return queue;
}

//sprawdzanie czy kolejka jest pusta
bool is_empty(queue_t *queue)
{
    return (queue->size == 0);
}

//sprawdzanie czy kolejka pełna
bool is_full(queue_t *queue)
{
    return (queue->size == queue->cap);
}

//dodawanie elementu do kolejki
queue_t *add(queue_t* queue, coord_t *item)
{
    //jeśli kolejka jest pełna to próbujemy ją zwiększyć
    if (is_full(queue))
    {
        queue = queue_resize(queue);
        if(!queue)
        {
            return NULL;
        }
    }
    //dodawanie elementu na koniec kolejki i ustawianie wskaźnika na ostatni element w kolejce
    queue->back = (queue->back + 1) % queue->cap;
    queue->array[queue->back] = item;
    queue->size = queue->size + 1;
    return queue;
}

//usuwanie elementu z kolejki
coord_t *pop(queue_t *queue)
{
    if (is_empty(queue))
    {
        return NULL;
    }
    //odczytywanie danych z początku kolejki i przesuwanie wskaźnika na początek kolejki do tyłu
    coord_t *res = (queue->array[queue->front]);
    queue->front = (queue->front + 1) % queue->cap;
    queue->size = queue->size - 1;
    return res;
}
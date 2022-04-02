#include <errno.h>
#include "queue.h"
#include "bitarray.h"
#include "types.h"

#ifndef PARSING_H_
#define PARSING_H_

arr_t *parce_R(char *token, array_with_length_t *dimentions, bool *memfail);

arr_t *parce_hex(char *token, array_with_length_t *dimentions, bool *memfail);

array_with_length_t *line_to_array(char *str, bool *memfail);

#endif

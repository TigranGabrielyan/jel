/**
* @file json_encode.c
*
* @brief The module contains JSON encoding functions.
*
*/
#include "json_encode.h"
#include "yajl_parse.h"

uint32_t
Sum(
    uint16_t x,
    uint16_t y)
{
    yajl_handle hYajl = NULL;
    yajl_free(hYajl);
    return x + y;
}

/**
* @file json_encode.c
*
* @brief The module contains JSON encoding functions.
*
*/
#include "json_encode.h"
#include "yajl_parse.h"

#include "apr_general.h"
#include "apr_pools.h"
#include "apr_hash.h"

uint32_t
Sum(
    uint16_t x,
    uint16_t y)
{
    yajl_handle hYajl = NULL;
    yajl_free(hYajl);

    apr_initialize();
    apr_pool_t * pool;
    apr_pool_create(&pool, NULL);
    apr_hash_t * ht = apr_hash_make(pool);

    apr_hash_clear(ht);
    apr_pool_destroy(pool);
    apr_terminate();

    return x + y;
}

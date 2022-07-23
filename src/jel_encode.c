/**
* @file jel_encode.c
*
* @brief The module contains implementation of encoding functions of JEL.
*
*/
#include "jel_encode.h"

#include "yajl_parse.h"

#include "apr_general.h"
#include "apr_pools.h"
#include "apr_hash.h"

typedef struct jel_encode_ctx_s
{
    jel_encode_callback_t   callback;
    void *                  user_context;
} jel_encode_ctx_t;

jel_result_t
jel_encode_start(
    jel_encode_callback_t   callback,
    void *                  user_context)
{
    if (NULL == callback)
    {
        return JEL_RESULT_INVALID_PARAM;
    }

    return JEL_RESULT_OK;
}

jel_result_t
jel_encode_do(const void * data, uint32_t length)
{
    return JEL_RESULT_OK;
}

jel_result_t
jel_encode_finish()
{
    return JEL_RESULT_OK;
}

static uint32_t
Sum(
    uint16_t x,
    uint16_t y)
{
    yajl_handle hYajl = yajl_alloc(NULL, NULL, NULL);
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

/**
* @file jel_encode.c
*
* @brief The module contains implementation of encoding functions of JEL.
*
*/
#include <stdlib.h>
#include <string.h>
#include "jel_encode.h"
#include "jel_tlv.h"

#include "yajl_parse.h"

#include "apr_general.h"
#include "apr_pools.h"
#include "apr_hash.h"

static yajl_handle init_yajl_parser();
static void uninit_yajl_parser(yajl_handle parser);

static apr_hash_t * init_apr_hashtable();
static void uninit_apr_hashtable(apr_hash_t * ht);
static void send_hashtable_keys();

static jel_result_t send_tlv(jel_tag_t tag, uint32_t length, const void * data);

typedef struct jel_encode_ctx_s
{
    jel_encode_callback_t   callback;
    void *                  user_context;

    yajl_handle             parser;

    apr_hash_t *            ht;

    uint32_t recent_key_id;
    uint8_t key_id_just_received;
} jel_encode_ctx_t;

jel_encode_ctx_t gCtx;

jel_result_t
jel_encode_start(
    jel_encode_callback_t   callback,
    void *                  user_context)
{
    if (NULL == callback)
    {
        return JEL_RESULT_INVALID_PARAM;
    }

    gCtx.callback = callback;
    gCtx.user_context = user_context;

    gCtx.parser = init_yajl_parser();
    if (NULL == gCtx.parser)
    {
        return JEL_RESULT_INIT_FAIL;
    }

    gCtx.ht = init_apr_hashtable();
    if (NULL == gCtx.ht)
    {
        return JEL_RESULT_INIT_FAIL;
    }

    return JEL_RESULT_OK;
}

jel_result_t
jel_encode_do(const void * data, uint32_t length)
{
    yajl_status status = yajl_status_ok;

    if (0 == length || NULL == data)
    {
        return JEL_RESULT_INVALID_PARAM;
    }

    status = yajl_parse(gCtx.parser, data, length);

    return yajl_status_ok == status ? JEL_RESULT_OK : JEL_RESULT_SYS_CALL_FAIL;
}

void
jel_encode_finish()
{
    send_hashtable_keys();

    uninit_yajl_parser(gCtx.parser);
    gCtx.parser = NULL;

    uninit_apr_hashtable(gCtx.ht);
    gCtx.ht = NULL;
}

static int yajl_null_cb(void *ctx)
{
    gCtx.key_id_just_received = 0;
    return 1;
}

static int yajl_boolean_cb(void * ctx, int value)
{
    return JEL_RESULT_OK == send_tlv(JEL_TLV_ELEMENT_BOOL, sizeof(value), &value) ? 1 : 0;
}

static int yajl_integer_cb(void *ctx, long long value)
{
    return JEL_RESULT_OK == send_tlv(JEL_TLV_ELEMENT_INTEGER, sizeof(value), &value) ? 1 : 0;
}

static int yajl_double_cb(void *ctx, double value)
{
    return JEL_RESULT_OK == send_tlv(JEL_TLV_ELEMENT_DOUBLE, sizeof(value), &value) ? 1 : 0;
}

static int yajl_string_cb(void *ctx, const unsigned char * value, size_t length)
{
    return JEL_RESULT_OK == send_tlv(JEL_TLV_ELEMENT_STRING, length, &value) ? 1 : 0;
}

static int yajl_map_key_cb(void *ctx, const unsigned char * stringVal, size_t stringLen)
{
    uint32_t * key_id = NULL;
    uint8_t *  key = NULL;

    if (1 == gCtx.key_id_just_received)
    {
        gCtx.callback(JEL_ENCODE_CB_STATUS_ERROR, NULL, 0, gCtx.user_context);
        return 0;
    }

    key = (uint8_t *) malloc(stringLen + 1);
    if (NULL == key)
    {
        gCtx.callback(JEL_ENCODE_CB_STATUS_ERROR, NULL, 0, gCtx.user_context);
        return 0;
    }
    key[stringLen] = 0;
    memcpy(key, stringVal, stringLen);

    key_id = apr_hash_get(gCtx.ht, key, stringLen + 1);
    if (NULL == key_id)
    {
        key_id = (uint32_t *) malloc(sizeof(uint32_t));
        if (NULL == key_id)
        {
            gCtx.callback(JEL_ENCODE_CB_STATUS_ERROR, NULL, 0, gCtx.user_context);
            return 0;
        }

        *key_id = apr_hash_count(gCtx.ht) + 1;
    }

    apr_hash_set(gCtx.ht, key, stringLen + 1, key_id);

    gCtx.recent_key_id = *key_id;
    gCtx.key_id_just_received = 1;

    return 1;
}

static int yajl_start_map_cb(void *ctx)
{
    return JEL_RESULT_OK == send_tlv(JEL_TLV_START, 0, NULL) ? 1 : 0;
}


static int yajl_end_map_cb(void *ctx)
{
    return JEL_RESULT_OK == send_tlv(JEL_TLV_END, 0, NULL) ? 1 : 0;
}

static yajl_callbacks callbacks = {
    yajl_null_cb,
    yajl_boolean_cb,
    yajl_integer_cb,
    yajl_double_cb,
    NULL,
    yajl_string_cb,
    yajl_start_map_cb,
    yajl_map_key_cb,
    yajl_end_map_cb,
    NULL,
    NULL
};

static yajl_handle
init_yajl_parser()
{
    yajl_handle parser = NULL;
    int config_result = 1;

    parser = yajl_alloc(&callbacks, NULL, NULL);
    if (NULL == parser)
    {
        goto cleanup;
    }

    config_result = yajl_config(parser, yajl_allow_comments, 1);
    if (0 == config_result)
    {
        goto cleanup;
    }

    config_result = yajl_config(parser, yajl_allow_trailing_garbage, 1);
    if (0 == config_result)
    {
        goto cleanup;
    }

    config_result = yajl_config(parser, yajl_allow_multiple_values, 1);

cleanup:
    if (0 == config_result)
    {
        yajl_free(parser);
        parser = NULL;
    }
    return parser;
}

static void
uninit_yajl_parser(yajl_handle parser)
{
    if (NULL != parser)
    {
        yajl_free(parser);
    }
}

static apr_hash_t *
init_apr_hashtable()
{
    apr_status_t status = APR_SUCCESS;
    apr_hash_t * ht = NULL;
    apr_pool_t * pool = NULL;

    status = apr_initialize();
    if (APR_SUCCESS != status)
    {
        return NULL;
    }

    status = apr_pool_create(&pool, NULL);
    if (APR_SUCCESS != status)
    {
        apr_terminate();
        return NULL;
    }

    ht = apr_hash_make(pool);
    if (NULL == ht)
    {
        apr_pool_destroy(pool);
        apr_terminate();
    }

    return ht;
}

static void
uninit_apr_hashtable(
    apr_hash_t * ht)
{
    if (NULL != ht)
    {
        apr_pool_t * pool = NULL;
        apr_hash_index_t * hi = NULL;
        void * key = NULL;
        void * val = NULL;

        for (hi = apr_hash_first(NULL, gCtx.ht); hi; hi = apr_hash_next(hi))
        {
            apr_hash_this(hi, (const void **) &key, NULL, &val);
            free(key);
            free(val);
        }

        pool = apr_hash_pool_get(ht);
        if (NULL != pool)
        {
            apr_pool_destroy(pool);
        }

        apr_terminate();
    }
}

static void
send_hashtable_keys()
{
    apr_hash_index_t * hi = NULL;
    void * val = NULL;
    void * key = NULL;
    apr_ssize_t key_len = 0;

    for (hi = apr_hash_first(NULL, gCtx.ht); hi; hi = apr_hash_next(hi))
    {
        apr_hash_this(hi, (const void **) &key, &key_len, &val);

        gCtx.recent_key_id = *((uint32_t *) val);
        if (JEL_RESULT_OK != send_tlv(JEL_TLV_KEY, key_len, key))
        {
            break;
        }
    }
}

static uint8_t
is_recent_key_id_valid_for_tag(
    jel_tag_t tag)
{
    uint8_t result = 0;
    switch (tag)
    {
        case JEL_TLV_START:
        case JEL_TLV_END:
        case JEL_TLV_KEY:
        {
            if (0 == gCtx.key_id_just_received)
            {
                result = 1;
            }
            break;
        }
        default:
        {
            result = gCtx.key_id_just_received;
        }
    }

    return result;
}

static jel_result_t
send_tlv(
    jel_tag_t       tag,
    uint32_t        length,
    const void *    data)
{
    jel_tlv_t * tlv = NULL;
    jel_result_t result = JEL_RESULT_OK;

    if (0 == is_recent_key_id_valid_for_tag(tag))
    {
        gCtx.callback(JEL_ENCODE_CB_STATUS_ERROR, NULL, 0, gCtx.user_context);
        return JEL_RESULT_INVALID_PARAM;
    }

    result = jel_tlv_construct(&tlv, tag, gCtx.recent_key_id, length, data);
    if (JEL_RESULT_OK != result)
    {
        gCtx.callback(JEL_ENCODE_CB_STATUS_ERROR, NULL, 0, gCtx.user_context);
    }
    else
    {
        gCtx.callback(JEL_ENCODE_CB_STATUS_OK, tlv, sizeof(*tlv) + tlv->length, gCtx.user_context);
        jel_tlv_destroy(tlv);
    }

    gCtx.key_id_just_received = 0;

    return result;
}

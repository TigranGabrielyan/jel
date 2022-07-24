#include <stdio.h>
#include "apr_file_io.h"
#include "jel_encode.h"

#define READ_CHUNK_SIZE ((uint32_t) 1024)
uint8_t data_buf[READ_CHUNK_SIZE];

typedef struct app_ctx_s
{
    apr_pool_t * pool;
    apr_file_t * out_file;
    apr_file_t * in_file;
} app_ctx_t;

void encode_callback(jel_encode_cb_status_t status, const void * data, uint32_t length, void * user_context);

apr_status_t apr_init(app_ctx_t * ctx);
void apr_uninit(app_ctx_t * ctx);

apr_status_t app_ctx_init(app_ctx_t * ctx);
void app_ctx_uninit(app_ctx_t * ctx);

void main()
{
    apr_status_t status = APR_SUCCESS;
    jel_result_t result = JEL_RESULT_OK;
    apr_ssize_t rb = READ_CHUNK_SIZE;
    app_ctx_t ctx;

    if (APR_SUCCESS != app_ctx_init(&ctx))
    {
        /* Error message already displayed. */
        return;
    }

    result = jel_encode_start(encode_callback, &ctx);
    if (JEL_RESULT_OK != result)
    {
        printf("main: jel_encode_start failed with error: %u.\n", result);
        app_ctx_uninit(&ctx);
        return;
    }

    while (rb == READ_CHUNK_SIZE)
    {
        status = apr_file_read(ctx.in_file, data_buf, &rb);
        if (APR_SUCCESS != status)
        {
            printf("main: apr_file_read failed with error: %u.\n", status);
            break;
        }

        result = jel_encode_do(data_buf, rb);
        if (JEL_RESULT_OK != result)
        {
            printf("main: jel_encode_do failed with error: %u.\n", result);
            break;
        }
    }

    jel_encode_finish();

    app_ctx_uninit(&ctx);
}

void
encode_callback(
    jel_encode_cb_status_t  status,
    const void *            data,
    uint32_t                length,
    void *                  user_context)
{
    if (JEL_ENCODE_CB_STATUS_OK == status)
    {
        apr_status_t status = APR_SUCCESS;
        apr_ssize_t size = length;
        app_ctx_t * ctx = (app_ctx_t *) user_context;

        status = apr_file_write(ctx->out_file, data, &size);
        if (APR_SUCCESS != status)
        {
            printf("encode_callback: apr_file_write failed to write %u bytes with error %u.\n", length, status);
        }

        if (length != size)
        {
            printf("encode_callback: apr_file_write wrote %u bytes while %u bytes are requested.\n", (uint32_t)size, length);
        }

    }
    else
    {
        printf("encode_callback: Error occured while parsing the input file.\n");
    }
}

apr_status_t apr_init(app_ctx_t * ctx)
{
    apr_status_t status = APR_SUCCESS;

    status = apr_initialize();
    if (APR_SUCCESS != status)
    {
        printf("apr_init: apr_initialize failed with error: %u.\n", status);
        return status;
    }

    status = apr_pool_create(&(ctx->pool), NULL);
    if (APR_SUCCESS != status)
    {
        printf("apr_init: apr_pool_create failed with error: %u.\n", status);
        apr_terminate();
    }

    return status;
}

void apr_uninit(app_ctx_t * ctx)
{
    if (NULL != ctx->pool)
    {
        apr_pool_destroy(ctx->pool);
    }

    apr_terminate();
}

apr_status_t app_ctx_init(app_ctx_t * ctx)
{
    apr_status_t status = APR_SUCCESS;
    const char * oname = "output";
    const char * iname = "input";

    status = apr_init(ctx);
    if (APR_SUCCESS != status)
    {
        /* Error message already displayed. */
        return status;
    }

    status = apr_file_open(
        &(ctx->out_file),
        oname,
        APR_FOPEN_CREATE | APR_FOPEN_WRITE | APR_FOPEN_BINARY,
        APR_OS_DEFAULT,
        ctx->pool);
    if (APR_SUCCESS != status)
    {
        printf("app_ctx_init: apr_file_open failed for %s file with error: %u.\n", oname, status);
        return status;
    }

    status = apr_file_open(
        &(ctx->in_file),
        iname,
        APR_FOPEN_READ,
        APR_OS_DEFAULT,
        ctx->pool);
    if (APR_SUCCESS != status)
    {
        printf("app_ctx_init: apr_file_open failed for %s file with error: %u.\n", iname, status);
    }

    return status;
}

void app_ctx_uninit(app_ctx_t * ctx)
{
    if (NULL != ctx->in_file)
    {
        apr_file_close(ctx->in_file);
    }

    if (NULL != ctx->out_file)
    {
        apr_file_close(ctx->out_file);
    }

    apr_uninit(ctx);
}

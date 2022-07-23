/**
* @file jel_tlv.c
*
* @brief The module contains TLV functions.
*
*/
#include <stdlib.h>
#include <string.h>
#include "jel_tlv.h"


jel_result_t
jel_tlv_construct(
    jel_tlv_t **    tlv,
    jel_tag_t       tag,
    uint32_t        id,
    uint32_t        length,
    const void *    data)
{
    jel_tlv_t * out = NULL;
    uint32_t size = sizeof(jel_tlv_t);

    if (NULL == tlv)
    {
        return JEL_RESULT_INVALID_PARAM;
    }

    /* Make sure that correct tag is provided as well as reserve space for Id if applied. */
    switch (tag)
    {
        case JEL_TLV_START:
        case JEL_TLV_END:
        {
            break;
        }
        case JEL_TLV_ELEMENT_BOOL:
        case JEL_TLV_ELEMENT_INTEGER:
        case JEL_TLV_ELEMENT_DOUBLE:
        case JEL_TLV_ELEMENT_STRING:
        {
            size += sizeof(id);
            break;
        }
        default:
        {
            return JEL_RESULT_INVALID_PARAM;
        }
    }

    size += length;

    out = (jel_tlv_t *) calloc(size, sizeof(uint8_t));
    if (NULL == out)
    {
        return JEL_RESULT_SYS_CALL_FAIL;
    }

    if (0 != length)
    {
        memcpy((void *)(out + 1), data, length);
    }

    out->tag = tag;
    *tlv = out;
    return JEL_RESULT_OK;
}

void
jel_tlv_destroy(
    jel_tlv_t * tlv)
{
    if (tlv != NULL)
    {
        free(tlv);
    }
}


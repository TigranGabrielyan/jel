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
    uint32_t size = length;
    uint8_t * data_ptr = NULL;
    uint8_t has_id = 0;

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
        case JEL_TLV_KEY:
        {
            size += sizeof(id);
            has_id = 1;
            break;
        }
        default:
        {
            return JEL_RESULT_INVALID_PARAM;
        }
    }

    out = (jel_tlv_t *) calloc(size + sizeof(jel_tlv_t), sizeof(uint8_t));
    if (NULL == out)
    {
        return JEL_RESULT_SYS_CALL_FAIL;
    }
    out->tag = tag;
    out->length = size;


    data_ptr = (uint8_t *) (out + 1);

    if (1 == has_id)
    {
        ((jel_tlv_element_t *) out)->id = id;

        data_ptr += sizeof(id);
    }

    if (0 != length)
    {
        memcpy(data_ptr, data, length);
    }

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


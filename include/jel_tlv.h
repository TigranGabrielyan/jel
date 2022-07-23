/**
* @file jel_tlv.h
*
* @brief Defines TLV interface for JSON files.
*
*/

#ifndef __jel_tlv_h__
#define __jel_tlv_h__

#include "jel_results.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef uint16_t jel_tag_t;
/* Definitions of the supported tags. */
#define JEL_TLV_UNKNOWN            ((jel_tag_t) 0)
#define JEL_TLV_START              ((jel_tag_t) 1)
#define JEL_TLV_END                ((jel_tag_t) 2)
#define JEL_TLV_ELEMENT_BOOL       ((jel_tag_t) 3)
#define JEL_TLV_ELEMENT_INTEGER    ((jel_tag_t) 4)
#define JEL_TLV_ELEMENT_DOUBLE     ((jel_tag_t) 5)
#define JEL_TLV_ELEMENT_STRING     ((jel_tag_t) 6)

#pragma pack(push, 1)
typedef struct jel_tlv_s
{
    jel_tag_t tag;
    uint32_t length;
} jel_tlv_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct jel_tlv_element_s
{
    jel_tlv_t header;
    uint32_t id;
    /* Followed by data. */
} jel_tlv_element_t;
#pragma pack(pop)


/*!
*******************************************************************************
** Construct TLV element based on provided data.
**
** @param[out]  tlv     Pinter where allocated TLV element will be stored.
** @param[in]   tag     Tag of the TLV element (one of JSON_TLV_***).
** @param[in]   id      Id associated with the TLV.
** @param[in]   length  Length of the data in the TLV element.
** @param[in]   data    Pointer where the data is stored.
**
** @return
**      - JEL_RESULT_TLV_OK
**      - JEL_RESULT_TLV_INVALID_PARAM
**      - JEL_RESULT_TLV_SYS_CALL_FAIL
*/
jel_result_t
jel_tlv_construct(
    jel_tlv_t **    tlv,
    jel_tag_t       tag,
    uint32_t        id,
    uint32_t        length,
    const void *    data);

/*!
*******************************************************************************
** Destroy TLV element and free its memory.
**
** @param[in]  tlv     Pinter of the constructed TLV element.
*/
void
jel_tlv_destroy(
    jel_tlv_t * tlv);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __jel_tlv_h__ */

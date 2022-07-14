/**
* @file json_encode.h
*
* @brief Defines interface for encoding JSON to TLV.
*
*/

#ifndef __json_encode_h__
#define __json_encode_h__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*!
*******************************************************************************
** Add two arguments and return the result.
**
** @param[in]   x       First argument
** @param[in]   y       Second argument
**
** @return
**      - Sum of two arguments
*/
uint32_t
Sum(
    uint16_t x,
    uint16_t y);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __json_encode_h__ */

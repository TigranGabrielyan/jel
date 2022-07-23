/**
* @file jel_encode.h
*
* @brief Defines encoding interfaces of JSON Encodding Library (JEL).
*
*/

#ifndef __jel_encode_h__
#define __jel_encode_h__

#include "jel_results.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void (*jel_encode_callback_t)(uint32_t status, const void * data, uint32_t length, void * user_context);

/*!
*******************************************************************************
** Start encoding process.
**
** @param[in]   callback        Callback function to be called when any data is ready.
** @param[in]   user_context    User context wich will be passed on each calbback call.
**
** @return
**      - JEL_RESULT_OK
**      - JEL_RESULT_INVALID_PARAM
**      - JEL_RESULT_SYS_CALL_FAIL
*/
jel_result_t
jel_encode_start(
    jel_encode_callback_t   callback,
    void *                  user_context);

/*!
*******************************************************************************
** Encode provided data.
**
** @param[in]   data    Data to be encoded.
** @param[in]   length  Length of the data to be encoded.
**
** @return
**      - JEL_RESULT_OK
**      - JEL_RESULT_INVALID_PARAM
*/
jel_result_t
jel_encode_do(const void * data, uint32_t length);

/*!
*******************************************************************************
** Finish encoding process.
**
** @return
**      - RESULT_OK
**      - RESULT_SYS_CALL_FAIL
*/
jel_result_t
jel_encode_finish();

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __jel_encode_h__ */

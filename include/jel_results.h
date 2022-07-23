/**
* @file jel_results.h
*
* @brief Defines result codes returned by JSON encoder library (JEL) interfaces.
*
*/

#ifndef __jel_results_h__
#define __jel_results_h__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef uint32_t jel_result_t;

/* Result codes */
#define JEL_RESULT_OK               ((jel_result_t) 0)
#define JEL_RESULT_INVALID_PARAM    ((jel_result_t) 1)
#define JEL_RESULT_SYS_CALL_FAIL    ((jel_result_t) 2)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __jel_results_h__ */

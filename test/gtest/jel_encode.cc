#include <gtest/gtest.h>
#include "jel_encode.h"
#include "jel_tlv.h"

void callback(
    jel_encode_cb_status_t status,
    const void * data,
    uint32_t length,
    void * user_context)
{
}

TEST(JEL_ENCODE, negative_jel_encode_start_null_callback_return_failure)
{
    EXPECT_EQ(JEL_RESULT_INVALID_PARAM, jel_encode_start(NULL, NULL));
}

TEST(JEL_ENCODE, negative_jel_encode_do_null_data_return_failure)
{
    EXPECT_EQ(JEL_RESULT_INVALID_PARAM, jel_encode_do(NULL, 1));
}

TEST(JEL_ENCODE, negative_jel_encode_do_0_length_return_failure)
{
    uint8_t data = 0;
    EXPECT_EQ(JEL_RESULT_INVALID_PARAM, jel_encode_do(&data, 0));
}

TEST(JEL_ENCODE, positive_jel_encode_start)
{
    EXPECT_EQ(JEL_RESULT_OK, jel_encode_start(callback, NULL));

    jel_encode_finish();
}

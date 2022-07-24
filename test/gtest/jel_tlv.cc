#include <gtest/gtest.h>
#include "jel_tlv.h"

TEST(JEL_TLV, negative_jel_tlv_construct_null_tlv_return_failure)
{
    EXPECT_EQ(JEL_RESULT_INVALID_PARAM, jel_tlv_construct(NULL, JEL_TLV_START, 0, 0, NULL));
}

TEST(JEL_TLV, negative_jel_tlv_construct_invalid_tag_return_failure)
{
    jel_tlv_t * tlv = NULL;
    EXPECT_EQ(JEL_RESULT_INVALID_PARAM, jel_tlv_construct(&tlv, JEL_TLV_UNKNOWN, 0, 0, NULL));
    EXPECT_EQ(JEL_RESULT_INVALID_PARAM, jel_tlv_construct(&tlv, JEL_TLV_KEY + 1, 0, 0, NULL));
}

TEST(JEL_TLV, positive_jel_tlv_construct_start_tag)
{
    jel_tlv_t * tlv = NULL;

    EXPECT_EQ(JEL_RESULT_OK, jel_tlv_construct(&tlv, JEL_TLV_START, 0, 0, NULL));

    EXPECT_EQ(JEL_TLV_START, tlv->tag);
    EXPECT_EQ(0, tlv->length);

    jel_tlv_destroy(tlv);
}

TEST(JEL_TLV, positive_jel_tlv_construct_end_tag)
{
    jel_tlv_t * tlv = NULL;

    EXPECT_EQ(JEL_RESULT_OK, jel_tlv_construct(&tlv, JEL_TLV_END, 0, 0, NULL));

    EXPECT_EQ(JEL_TLV_END, tlv->tag);
    EXPECT_EQ(0, tlv->length);

    jel_tlv_destroy(tlv);
}

TEST(JEL_TLV, positive_jel_tlv_construct_bool_tag)
{
    jel_tlv_t * tlv = NULL;
    uint8_t value = 1;
    uint32_t id = 7;
    uint8_t * ptr = NULL;

    EXPECT_EQ(JEL_RESULT_OK, jel_tlv_construct(&tlv, JEL_TLV_ELEMENT_BOOL, id, sizeof(value), &value));

    EXPECT_EQ(JEL_TLV_ELEMENT_BOOL, tlv->tag);
    EXPECT_EQ(sizeof(value) + sizeof(id), tlv->length);

    ptr = (uint8_t *)(tlv + 1);
    EXPECT_EQ(id, *((uint32_t*) ptr));
    EXPECT_EQ(value, *(ptr + sizeof(uint32_t)));

    jel_tlv_destroy(tlv);
}

TEST(JEL_TLV, negative_jel_tlv_construct_bool_tag_invalid_data_return_failure)
{
    jel_tlv_t * tlv = NULL;
    uint32_t id = 7;

    EXPECT_EQ(JEL_RESULT_INVALID_PARAM, jel_tlv_construct(&tlv, JEL_TLV_ELEMENT_BOOL, id, 1, NULL));
    EXPECT_EQ(JEL_RESULT_INVALID_PARAM, jel_tlv_construct(&tlv, JEL_TLV_ELEMENT_BOOL, id, 0, &id));
}


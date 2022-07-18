ADD_EXECUTABLE(json_encoder_test
    json_encode.cc
)

TARGET_LINK_LIBRARIES(json_encoder_test
    gtest
    gtest_main
    pthread
)

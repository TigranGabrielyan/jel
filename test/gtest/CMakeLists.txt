ADD_EXECUTABLE(jel_test
    jel_encode.cc
    jel_tlv.cc
)

TARGET_LINK_LIBRARIES(jel_test
    gtest
    gtest_main
    pthread
    jel
    yajl
    apr-1
)

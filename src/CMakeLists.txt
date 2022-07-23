SET(LIB_NAME jel)

SET(SRC_FILES jel_encode.c jel_tlv.c)

ADD_LIBRARY(${LIB_NAME} ${SRC_FILES})
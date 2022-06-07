#ifndef ASM_SRC_UTIL_BYTEARRAY_HPP_
#define ASM_SRC_UTIL_BYTEARRAY_HPP_

#include <ctype.h>

struct ByteArray {
    void *data_      = nullptr;
    size_t size_     = 0;
    size_t capacity_ = 0;
};

void PushBack(ByteArray *arr, const void *data, size_t lenght);

size_t GetArraySize(ByteArray *arr);

void* ByteArrayAt(ByteArray *arr, size_t i);

void ByteArrayClear(ByteArray *arr);

#endif // ASM_SRC_UTIL_BYTEARRAY_HPP_
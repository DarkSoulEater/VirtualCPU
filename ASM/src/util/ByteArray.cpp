#include <stdlib.h>
#include <assert.h>
#include <iostream>

#include "ByteArray.hpp"

const size_t kByteArrayDefaultSize = 64;

static void Reallocate(ByteArray *arr) {
    if (arr->data_ == nullptr) {
        arr->size_ = 0;
        arr->capacity_ = kByteArrayDefaultSize;
        arr->data_ = calloc(1, arr->capacity_);
        return;
    }

    arr->capacity_ *= 2;
    arr->data_ = realloc(arr->data_, arr->capacity_);
}

void PushBack(ByteArray *arr, const void *data, size_t lenght) {
    assert(arr);
    assert(data);
    
    while (arr->size_ + lenght > arr->capacity_) {
        Reallocate(arr);
    }
    
    char* arr_data = (char*)arr->data_ + arr->size_;
    arr->size_ += lenght;

    size_t i = 0;
    for ( ; (i + 1) * 8 <= lenght; ++i) {
        *((size_t*)arr_data + i) = *((size_t*)data + i);
    }

    for (i *= 8; i < lenght; ++i) {
        *((char*  )arr_data + i) = *((char*  )data + i);
    }
}

size_t GetArraySize(ByteArray *arr) {
    return arr->size_;
}

void* ByteArrayAt(ByteArray *arr, size_t i) {
    return (char*)arr->data_ + i;
}

void ByteArrayClear(ByteArray *arr) {
    free(arr->data_);
    arr->size_ = arr->capacity_ = 0;
}

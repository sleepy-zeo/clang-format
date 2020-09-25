#include "sz_string.h"

#include <cmath>
#include <cstring>

const static size_t INIT_CAPACITY = 32;

sleepy::string::wrapper::wrapper(const char* data) {
    printf("wrapper\n");
    mm_refCount = 1;
    if (data == nullptr) {
        mm_capacity = INIT_CAPACITY;
        mm_capacityAllocated = mm_capacity + 1;
        mm_data = mm_allocatorChar.allocate(mm_capacityAllocated);
        memset(mm_data, '\0', mm_capacityAllocated);
        return;
    }
    int data_len = strlen(data);
    if (data_len == 0) {
        mm_capacity = INIT_CAPACITY;
    } else {
        if (data_len > INIT_CAPACITY) {
            mm_capacity = std::ceil(1.0 * data_len / INIT_CAPACITY) * INIT_CAPACITY;
        } else {
            mm_capacity = INIT_CAPACITY;
        }
    }
    mm_capacityAllocated = mm_capacity + 1;
    mm_data = mm_allocatorChar.allocate(mm_capacityAllocated);
    memset(mm_data, '\0', mm_capacityAllocated);
    memcpy(mm_data, data, data_len);
}

sleepy::string::wrapper::~wrapper() {
    printf("~wrapper\n");
    memset(mm_data, '\0', strlen(mm_data));
    mm_allocatorChar.deallocate(mm_data, mm_capacityAllocated);
}

void sleepy::string::wrapper::resize(int n) {
    if (n < 0) {
        throw std::runtime_error("resize num should not less than 0");
    }
    if (n != mm_capacity) {
        int _mm_capacity = n;
        int _mm_capacityAllocated = _mm_capacity + 1;
        char* _mm_data = mm_allocatorChar.allocate(_mm_capacityAllocated);
        int data_len = strlen(mm_data);
        memset(_mm_data, '\0', _mm_capacityAllocated);
        memcpy(_mm_data, mm_data, data_len > n ? n : data_len);
        mm_allocatorChar.deallocate(mm_data, mm_capacityAllocated);
        mm_data = _mm_data;
        mm_capacity = _mm_capacity;
        mm_capacityAllocated = _mm_capacityAllocated;
    }
}

sleepy::string::string() {
    m_wrapper = new wrapper();
}

sleepy::string::string(const char* data) {
    m_wrapper = new wrapper(data);
}

sleepy::string::string(const sleepy::string& rhs) {
    m_wrapper = rhs.m_wrapper;
    m_wrapper->mm_refCount++;
}

sleepy::string::~string() {
    if (--m_wrapper->mm_refCount == 0) {
        delete m_wrapper;
    }
}

sleepy::string& sleepy::string::operator=(const sleepy::string& rhs) {
    if (this == &rhs) {
        return *this;
    }

    m_wrapper->mm_refCount--;
    m_wrapper = rhs.m_wrapper;
    m_wrapper->mm_refCount++;
    return *this;
}

void sleepy::string::resize(int n) {
    m_wrapper->resize(n);
}

void sleepy::string::debug() {
    printf(
        "debug: \n"
        "   m_wrapper addr: %p\n"
        "   mm_refCount: %zu\n"
        "   mm_capacity: %zu\n"
        "   mm_data: %s\n"
        "==========\n",
        m_wrapper,
        m_wrapper->mm_refCount,
        m_wrapper->mm_capacity,
        m_wrapper->mm_data);
}

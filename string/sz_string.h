#ifndef CXX_UTILITIES_STRING_STRING_H_
#define CXX_UTILITIES_STRING_STRING_H_

#include <memory>
namespace sleepy {

class string {

public:
    string();
    explicit string(const char* data);
    string(const string& rhs);
    ~string();

    string& operator=(const string& rhs);

    void resize(int n);

    void debug();

private:
    class wrapper {
    public:
        explicit wrapper(const char* data = nullptr);
        ~wrapper();

        void resize(int n);

    private:
        friend string;
        static std::allocator<char> mm_allocatorChar;

        size_t mm_refCount;

        char* mm_data;
        size_t mm_capacity;
        size_t mm_capacityAllocated;  // mm_capacity + 1
    };

    wrapper* m_wrapper;
};

std::allocator<char> sleepy::string::wrapper::mm_allocatorChar;

}  // namespace sleepy

#endif  // CXX_UTILITIES_STRING_STRING_H_

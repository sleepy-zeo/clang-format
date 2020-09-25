# Cpp Utilities

## clang-format

Use file [.clang-format](.clang-format) to format cpp files

## string

std::string的简单版本的实现[sz_string.h](string/sz_string.h)

## std::allocator

```c++
std::allocator<std::string> allocator_string;

// 分配占用的内存空间，不会进行其他任何操作
auto* begin = allocator_string.allocate(5);
// 释放占用的内存空间，不会进行其他任何操作
// 参数一必须是allocate返回的指针，参数二必须是allocate传入的值
allocator_string.deallocate(begin, 5);
```
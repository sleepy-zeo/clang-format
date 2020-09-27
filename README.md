# Cpp Utilities

## clang-format

格式化cpp文件的工具
- [.clang-format](.clang-format)

## string

std::string的简单版本的实现，详细参考：
- [sz_string.h](string/sz_string.h)
- [sz_string.cpp](string/sz_string.cpp)

## std::allocator

```c++
std::allocator<std::string> allocator_string;

// 分配占用的内存空间，不会进行其他任何操作
auto* begin = allocator_string.allocate(5);
// 释放占用的内存空间，不会进行其他任何操作
// 参数一必须是allocate返回的指针，参数二必须是allocate传入的值
allocator_string.deallocate(begin, 5);
```

## volatile

可见性：
<br>每个线程都有自己的缓存空间，对变量的操作都是在缓存中进行的，之后再将修改后的值返回到主存中，
<br>有可能一个线程在将共享变量修改后，还没有来的及将缓存中的变量返回给主存中，另外一个线程就对共享变量进行修改，那么这个线程拿到的值是主存中未被修改的值

原子性：
<br>对基本类型变量的赋值和读取是原子操作
<br>但是像j=i或者i++这样的操作都不是原子操作，因为他们都进行了多次原子操作，两个原子操作加起来就不是原子操作了

volatile保证了可见性但是不保证原子性：
<br>当一个共享变量被volatile修饰时，它会保证修改的值会立即被更新到主存，当有其他线程需要读取时，读取到的一定最新的值，而不会是缓存值
<br>所以：
- 如果一个变量被volatile修饰了，那么肯定可以保证每次读取这个变量值的时候得到的值是最新的
- 但是如果对这个变量进行了非原子操作，就无法保证变量的原子性了

# fsm(finite state machine)



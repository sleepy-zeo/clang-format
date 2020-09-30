#include <iostream>
#include <memory>

/// 模板类
template <typename T>
class Class {
public:
    T member;
};

/// 类模板的特化与偏特化
/// if ... else ... 结构

// 特化的一个简单的例子
// 原型
template <typename T>
class AddFloatOrMulInt {};

// AddFloatOrMulInt的int特化
template <>
class AddFloatOrMulInt<int> {
public:
    static int Do(int a, int b) {
        return a * b;
    }
};

// AddFloatOrMulInt的float特化
template <>
class AddFloatOrMulInt<float> {
public:
    static float Do(float a, float b) {
        return a + b;
    }
};

// 特化的另一个例子
template <typename T>
class TypeToID {};

// float类型的特化
template <>
class TypeToID<float> {
public:
    static int const ID = 1;
};

// 匹配任意类型的指针的特化
template <typename T>
class TypeToID<T*> {
public:
    static int const ID = 0x8000;
};

/// 利用上面的匹配任意类型的指针特性
// RemovePointer可以将任意层数的指针的去掉
template <typename T>
class RemovePointer {
public:
    using Result = T;
};

template <typename T>
class RemovePointer<T*> {
public:
    using Result = typename RemovePointer<T>::Result;
};

/// 名称查找
/// 如果名字查找和模板参数T有关，那么查找会延期到模板参数全都确定的时候
/// 如果名字查找和模板参数T无关，那么在模板定义处，就应该找得到这个名字的声明
// 两阶段名称查找，第一阶段检查非依赖的名称，第二阶段检查依赖的名称
int a;
struct B {
    int v;
};
template <typename T>
struct X {
    B b;
    typedef int Y;
    Y y;
    void foo() {
        // 非依赖，直接在第一阶段检查
        b.v += y;
        // 依赖，会延期到第二阶段检查检查
        b.v *= T::s_mem;
    }
};
// 名称查找的另一个例子
template <typename T>
struct Y {
    // X<T>是一个依赖性名称，模板定义阶段并不管X<T>是不是正确的
    typedef X<T> ReboundType;

    // X<T>是一个依赖性名称，X<T>::MemberType也是一个依赖性名称，同上
    typedef typename X<T>::MemberType MemberType2;

    // UnknownType 不是一个依赖性名称，由于UnknownType不存在，所以直接报错
    // typedef UnknownType MemberType3;
};

/// typename
/// typename T::innerClass obj;
/// 这里typename的作用是告诉编译器innerClass是T中的一个类，而不是T的一个静态变量
template <typename T>
struct M {};
template <typename T>
struct G {
    // 不知道M<T>::type的信息，通过typename明确告知编译器type是M<T>中的一个类型
    typedef typename M<T>::type I;
};

/// 深入类模板的特化与偏特化
// clang-format off
// 在特化的时候，当所有类型都已经确定，我们就可以抛弃全部的模板参数，写出template <> struct X<int, float>这样的形式
template <typename T, typename U> struct D            ;    // 0
template <typename T>             struct D<T,  T  > {};    // 1
template <typename T>             struct D<T*, T  > {};    // 2
template <typename T>             struct D<T,  T* > {};    // 3
template <typename U>             struct D<U,  int> {};    // 4
template <typename U>             struct D<U*, int> {};    // 5
template <typename U, typename T> struct D<U*, T* > {};    // 6
template <typename U, typename T> struct D<U,  T* > {};    // 7
template <typename T>             struct D<std::unique_ptr<T>, std::shared_ptr<T>>; // 8

D<float*,  int>      _v0;
D<double*, int>      _v1;
D<double,  double>   _v2;
D<float*,  double*>  _v3;
//D<float*,  float*>   _v4;
D<double,  float*>   _v5;
D<int,     double*>  _v6;
//D<int*,    int>      _v7;
D<double*, double>   _v8;
// clang-format on

/// SFINAE(substitution failure is not an error)
namespace sleepy {
// C++11
template <bool, typename T = void>
struct enable_if {};

template <typename T>
struct enable_if<true, T> {
    using type = T;
};

// C++14
template <bool B, typename T = void>
using enable_if_t = typename enable_if<B, T>::type;

template <typename T, typename U>
struct is_same {
    constexpr static bool value = false;
};

template <typename T>
struct is_same<T, T> {
    constexpr static bool value = true;
};

template <class... Ts>
struct tuple {};

// compiler优先选择特化版本
template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {

    // cons
    explicit tuple(T t, Ts... ts) : tuple<Ts...>(ts...), tail(t) {
    }

    T tail;
};

template <size_t, class>
struct elem_type_holder {};

template <class T, class... Ts>
struct elem_type_holder<0, tuple<T, Ts...>> {
    using type = T;
};

template <size_t k, class T, class... Ts>
struct elem_type_holder<k, tuple<T, Ts...>> {
    using type = typename elem_type_holder<k - 1, tuple<Ts...>>::type;
};

template <size_t k, class... Ts>
typename sleepy::enable_if<k == 0, typename elem_type_holder<0, tuple<Ts...>>::type>::type get(tuple<Ts...> t) {
    return t.tail;
}

template <size_t k, class T, class... Ts>
typename sleepy::enable_if<k != 0, typename elem_type_holder<k, tuple<T, Ts...>>::type>::type get(tuple<T, Ts...> t) {
    // 为什么可以这样???
    tuple<Ts...> base = t;
    return get<k - 1, Ts...>(base);
}

template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
typename std::remove_reference<T>::type&& move(T&& param) {
    using ReturnType = typename std::remove_reference<T>::type&&;
    return static_cast<ReturnType>(param);
}

template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type& __t) noexcept {
    return static_cast<T&&>(__t);
}

template <typename T>
constexpr T&& forward(typename std::remove_reference<T>::type&& __t) noexcept {
    static_assert(!std::is_lvalue_reference<T>::value, "template argument substituting T is an lvalue reference type");
    return static_cast<T&&>(__t);
}

}  // namespace sleepy

// 当sleepy::enable_if<bool, T>的bool为false时，sleepy::enable_if<false, T>::value会出现替换错误，因此该模板就不会被选择
template <class T, typename sleepy::enable_if<std::is_integral<T>::value, T>::type* = nullptr>
void do_stuff(T& t) {
    std::cout << "do_stuff integral\n";
    // an implementation for integral types (int, char, unsigned, etc.)
}

template <class T, typename sleepy::enable_if<std::is_class<T>::value, T>::type* = nullptr>
void do_stuff(T& t) {
    std::cout << "do_stuff class\n";
    // an implementation for class types
}

template <
    class T,
    typename sleepy::enable_if<!std::is_class<T>::value && !std::is_integral<T>::value, T>::type* = nullptr>
void do_stuff(T& t) {
    std::cout << "do_stuff nothing\n";
}

/// 变参模板
// Ts不是一个类型，而是一个不定常的类型列表
template <typename... Ts>
class L {};

template <typename T>
T add(T v) {
    return v;
}

template <typename T, typename... Args>
T add(T v, Args&&... args) {
    return v + add(args...);
}

template <typename T>
bool pair_comparer(T a, T b) {
    return a == b;
}

template <typename T>
bool pair_comparer(T) {
    return false;
}

template <typename T, typename... Args>
bool pair_comparer(T a, T b, Args... args) {
    return a == b && pair_comparer(args...);
}

template <typename T>
void print_f(T t) {
    std::cout << t << std::endl;
}

template <typename T, typename... Args>
void print_f(T t, Args... args) {

    std::cout << t << " ";
    print_f(args...);
}

template <template <typename, typename...> class ContainerType, typename ValueType, typename... Args>
void printContainer(const ContainerType<ValueType, Args...>& c) {
    for (const auto& v : c) {
        std::cout << v << ' ';
    }
    std::cout << '\n';
}

int main() {

    std::cout << std::boolalpha;

    std::cout << sleepy::is_same<int, int>::value << std::endl;

    std::cout << add(1, 2, 3, 4, 5, 1.1, 'c', 12.3f) << std::endl;

    std::cout << add(std::string("ab"), std::string("cd")) << std::endl;

    std::cout << pair_comparer(1.5, 1.5, 2, 2, 'c', 'd') << std::endl;

    print_f(1, 2, 3, "abcd", '*', std::string("abcd"));

    sleepy::tuple<double, uint64_t, const char*> t1(12.2, 42, "big");

    sleepy::tuple<const char*> t2 = t1;

    std::cout << t2.tail << std::endl;

    sleepy::elem_type_holder<2, sleepy::tuple<double, uint64_t, const char*>>::type str;

    std::cout << sleepy::get<1>(t1) << std::endl;

    return 0;
}
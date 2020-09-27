#include <iostream>

template <unsigned long N>
struct binary {
    static unsigned const value = binary<N / 10>::value * 2 + N % 10;
};

template <>
struct binary<1> {
    static unsigned const value = 1;
};

template <>
struct binary<0> {
    static unsigned const value = 0;
};

template <class X, class arg1,class arg2>
X apply(X x,arg1 arg_1,arg2 arg_2){
    return x(arg_1,arg_2);
}

int main() {

    std::cout << binary<10010>::value << std::endl;

    return 0;
}
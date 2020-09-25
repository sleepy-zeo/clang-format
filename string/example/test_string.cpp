#include <cmath>

#include <sz_string.h>

#include "iostream"

int main() {

    sleepy::string str("abcdefghijk");
    printf("%p\n", &str);

    str.debug();

    {
        sleepy::string b = str;
        printf("%p\n", &b);
        b.debug();

        sleepy::string c = b;
        printf("%p\n", &c);

        c.resize(2);
        c.debug();

        str.resize(20);
        str.debug();
    }

    str.resize(39);
    str.debug();

    printf("%p\n", &str);

    return 0;
}
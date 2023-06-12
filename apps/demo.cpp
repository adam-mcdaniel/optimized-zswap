#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <algorithm>
#include <cmath>
#include "zsmalloc.hpp"
#include "zsallocator.hpp"

class Foo {
public:
    std::string s = "";

    Foo() {
        for (int i = 0; i < 10; ++i) {
            s += std::rand() % 26 + 'a';
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Foo& foo) {
        return os << foo.s;
    }
};


class Bar {
public:
    std::string s = "";

    Bar() {
        for (int i = 0; i < 10; ++i) {
            s += std::rand() % 26 + 'a';
        }
    }

    bool operator<(const Bar rhs) const {
        return s < rhs.s;
    }

    friend std::ostream& operator<<(std::ostream& os, const Bar& bar) {
        return os << bar.s;
    }
};

class Baz {
public:
    std::string s = "";

    void randomize() {
        for (int i = 0; i < 10; ++i) {
            s += std::rand() % 26 + 'a';
        }
    }

    friend std::ostream& operator<<(std::ostream& os, const Baz& baz);
};

std::ostream& operator<<(std::ostream& os, const Baz& baz) {
    return os << baz.s;
}
int main() {
    std::srand(std::time(nullptr));

    std::vector<Foo, Mallocator<Foo>> v(0);
    for (int i = 0; i < 100; ++i)
        v.push_back(Foo());
    std::vector<Foo, Mallocator<Foo>> v2(v.begin(), v.end());

    // std::vector<Bar, Mallocator<Bar>> v3;
    // for (int i = 0; i < 100; ++i)
    //     v3.push_back(Bar());

    std::vector<Baz, Mallocator<Baz>> v4;
    for (int i = 0; i < 100; ++i) {
        Baz b;
        b.randomize();
        v4.push_back(b);
    }


    std::cout << is_type_reflectable<Foo>() << std::endl;
    std::cout << is_type_reflectable<Bar>() << std::endl;
    std::cout << is_type_reflectable<Baz>() << std::endl;
}
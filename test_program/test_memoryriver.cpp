#include "../src/MemoryRiver.hpp"
#include<bits/stdc++.h>
using namespace std;

int main() {
    MemoryRiver<long long, 2> test;
    test.initialise("test");
    test.write_info(23, 1);
    test.write_info(34, 2);
    int tmp;
    test.get_info(tmp, 1);
    cout << tmp << '\n';
    test.get_info(tmp, 2);
    cout << tmp << '\n';
    long long x = 44, tmp2;
    int index = test.write(x);
    test.read(tmp2, index);
    auto c = x + 2;
    int index2 = test.write(c);
    cout << tmp2 << '\n';
    test.update(c, index);
    test.read(tmp2, index);
    cout << tmp2 << '\n';
    test.read(tmp2, index2);
    cout << index << '\n';
    test.Delete(index);
    index = test.write(c);
    cout << index << '\n';
    return 0;
}
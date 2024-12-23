#include "MemoryRiver.hpp"
#include<bits/stdc++.h>

using std::cout;

int main()
{
    puts("qwq");
    MemoryRiver<int, 0> file_1;
    file_1.initialise("file1");
    int a = 233, b, c;
    cout << (b = file_1.write(a)) << '\n';
    a = 344;
    int e = 3333, d;
    cout <<  (c = file_1.write(a)) << '\n';
    file_1.Delete(c);
    cout << (d = file_1.write(e)) << '\n';
    file_1.read(e, d); cout << e << '\n';
    file_1.Delete(d);
    cout << (e = file_1.write(a)) << '\n';
    return 0;
}
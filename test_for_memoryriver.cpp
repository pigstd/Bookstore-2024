#include "MemoryRiver.hpp"
#include<bits/stdc++.h>

using std::cout;

int main()
{
    MemoryRiver<int,10> file_1;
    file_1.initialise("file1");
    for (int i = 1; i <= 10; i++)
        file_1.write_info(i, i);
    for (int i = 10; i >= 1; i--)
    {
        int k;
        file_1.get_info(k, i);
        cout << k << ' ';
    }
    int a = 233, b, c;
    cout << (b = file_1.write(a)) << '\n';
    a = 344;
    cout <<  (c = file_1.write(a)) << '\n';
    file_1.read(a, b);
    cout << a << '\n';
    file_1.read(b, c);
    cout << b << '\n';
    return 0;
}
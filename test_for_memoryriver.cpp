#include "MemoryRiver.h"
#include<bits/stdc++.h>

using std::cout;

int main()
{
    MemoryRiver<int,10> file_1;
    file_1.initialise("file1");
    int a = 233, b;
    cout << (b = file_1.write(a)) << '\n';
    a = 344;
    cout << file_1.write(a) << '\n';
    file_1.read(a,b);
    cout << a << '\n';
    return 0;
}
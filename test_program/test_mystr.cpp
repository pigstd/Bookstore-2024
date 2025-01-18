#include "../src/mystr.hpp"
#include <bits/stdc++.h>
using namespace std;

int main() {
    str<40> str1("233"), str2("344");
    cout << str1 << ' ' << str2 << '\n';
    cout << (str1 == str2) << ' ' << (str1 < str2) << ' ' << (str2 < str1) << '\n';
    return 0;
}
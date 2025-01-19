#include "../src/mystr.hpp"
#include <bits/stdc++.h>
using namespace std;

bool checknum(char ch) {
    return ch <= '9' && ch >= '0';
}

int main() {
    str<40> str1("233"), str2("344");
    cout << str1 << ' ' << str2 << '\n';
    cout << (str1 == str2) << ' ' << (str1 < str2) << ' ' << (str2 < str1) << '\n';
    // cout << checknum('2') << '\n';
    cout << str1.check(checknum) << '\n';
    str2 = str<40>("abc");
    cout << str2.check(checknum) << '\n';
    return 0;
}
#include "../src/mystr.hpp"
#include <bits/stdc++.h>
using namespace std;

bool checknum(char ch) {
    return ch <= '9' && ch >= '0';
}

int main() {
    str<40> str1("233"), str2("344");
    string qwq = str1;
    cout << qwq << ' ' << qwq.size() << '\n';
    cout << str1 << ' ' << str2 << '\n';
    cout << (str1 == str2) << ' ' << (str1 < str2) << ' ' << (str2 < str1) << '\n';
    // cout << checknum('2') << '\n';
    cout << str1.check(checknum) << '\n';
    try {
        str2 = str<40>("abc", checknum);
    }
    catch (exception &x) {
        cout << x.what() << '\n';
    }
    cout << str2.check(checknum) << '\n';
    return 0;
}
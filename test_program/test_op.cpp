#include<bits/stdc++.h>
#include "../src/operator.hpp"
using namespace std;

int main() {
    cerr << "start test op\n";
    init();
    //登录栈，分别是用户以及选择的书的 id
    Loginstack LoginStack; LoginStack.push_back({4, -1});
    string optstr;
    while(getline(cin, optstr)) {
        try {
            operation(LoginStack, optstr);
        }
        catch(exception &err) {
            cout << err.what() << '\n';
        }
    }
    optstr = "quit";
    operation(LoginStack, optstr);
    return 0;
}
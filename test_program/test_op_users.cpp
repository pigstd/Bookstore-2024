#include<bits/stdc++.h>
#include "../src/operator.hpp"
using namespace std;

int main() {
    // User root("root", "sjtu", "7", "admin");
    // MemoryRiver<User, 0> fileuser;
    // fileuser.initialise("User", 0, 1);
    // cout << fileuser.write(root) << '\n';
    init();
    User nowuser;
    nowuser.useradd();
    //登录栈，分别是用户以及选择的书的 id
    Loginstack LoginStack; LoginStack.push_back({nowuser.getID_int(), -1});
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
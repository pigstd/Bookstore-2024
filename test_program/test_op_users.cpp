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
    // root.print();
    try {
        string s = "su root sjtu";
        auto orders = split_string(s);
        su(nowuser, orders);
        nowuser.print();
        s = "logout", orders = split_string(s);
        logout(nowuser, orders);
        nowuser.print();
    }
    catch(exception &err) {
        cout << err.what() << '\n';
    }
}
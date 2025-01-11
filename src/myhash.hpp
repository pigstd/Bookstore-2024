#ifndef MYHASH
#define MYHASH

#include <string>
#include <iostream>

class hash {
private:
    int hs1, hs2;
    static int base1, base2, mod1, mod2;
public:
    hash(const std::string &s = "") {
        hs1 = hs2 = 0;
        for (auto c : s)
            hs1 = (1ll * hs1 * base1 + (long long)(c) + 2ll) % mod1,
            hs2 = (1ll * hs2 * base2 + (long long)(c) + 2ll) % mod2;
    }
    bool operator < (const hash &b) const {return hs1 < b.hs1 || (hs1 == b.hs1 && hs2 < b.hs2);};
    bool operator == (const hash &b) const {return hs1 == b.hs1 && hs2 == b.hs2;};
    void show() const {
        std::cerr << hs1 << ' ' << hs2 << '\n';
    }
};
int hash::base1 = 131;
int hash::base2 = 137;
int hash::mod1 = 1e9 + 7;
int hash::mod2 = 998244353;

#endif //MYHASH
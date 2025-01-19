#ifndef USER
#define USER

#include "mystr.hpp"
#include "MemoryRiver.hpp"
#include "Exception.hpp"
#include <string>
#include <cctype>

using std::string;

enum usertype {
    visitor = 0,
    customer = 1,
    employee = 3,
    owner = 7
};

#define userstr str<30>

bool isnum(char ch) {return ch <= '9' && ch >= '0';}// 判断字符是否是数字
bool isletter(char ch) {return (ch <= 'Z' && ch >= 'A') || (ch <= 'z' && ch >= 'a');}

bool isvalidname(char ch) {return ch == '_' || isnum(ch) || isletter(ch);}
bool is_visible(char ch) {return std::isprint(ch);}

bool checkPrivilege(const string &s) {
    if (s.size() != 1) return 0;
    if (s[0] != '1' && s[0] != '3' && s[0] != '7') return 0;
    return 1;
}

class User {
public:
    int UserID_int;
    usertype userType;
    userstr UserID, Password, Username;
public:
    User(string _UserID, string _Password, string Privilege, string _Username) {
        UserID_int = -1;// UserID_int 是在文件里存储的位置，一开始不知道
        if (checkPrivilege(Privilege) == false) throw Invalid();
        userType = usertype(Privilege[0] - '0');
        UserID = userstr(_UserID, is_visible),
        Password = userstr(_Password, isvalidname),
        Username = userstr(_Username, isvalidname);
    }
    //将用户添加进文件 file，如果 UserID_int 是 -1，则分配 UserID_int
    void useradd(MemoryRiver<User> &file) {
        if (UserID_int == -1)
            UserID_int = file.write(*this);
        file.update(*this, UserID_int);
    }
};

#endif //USER
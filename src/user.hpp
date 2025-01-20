#ifndef USER
#define USER

#include "mystr.hpp"
#include "MemoryRiver.hpp"
#include "Exception.hpp"
#include "database.hpp"
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
private:
    int UserID_int, LoginTime;
    usertype userType;
    userstr UserID, Password, Username;
public:
    // 生成游客
    User(): UserID(), Password(), Username() {LoginTime = 0, UserID_int = -1, userType = visitor;}
    // 不指定 Privilege，类型是 customer
    User(string _UserID, string _Password, string _Username) {
        UserID_int = -1;// UserID_int 是在文件里存储的位置，一开始不知道
        userType = customer;
        LoginTime = 0;
        UserID = userstr(_UserID, isvalidname),
        Password = userstr(_Password, isvalidname),
        Username = userstr(_Username, is_visible);
    }
    // 指定 Privilege
    User(string _UserID, string _Password, string Privilege, string _Username) :
    User(_UserID, _Password, _Username) {
        if (checkPrivilege(Privilege) == false) throw Invalid();
        userType = usertype(Privilege[0] - '0');
    }
    //显示 userType
    usertype gettype() const {return userType;}
    /*
    修改 UserID_int 位置上的用户信息
    存入文件 User
    */
    void userupd();
    /*
    将用户添加进文件 User 和键值数据库 UserID_to_int
    若用户 ID 已经存在则抛出异常
    */
    void useradd();
    // 修改用户的密码并且更新数据库
    void changepassword(string, string);
    // 修改用户的密码并且更新数据库（不给出 currentpassword）
    void changepassword(string);
    //判断密码是否正确
    bool checkpassword(string _password) {return Password == userstr(_password, isvalidname);}
    // 判断是否在登录栈里面
    bool isLogin() const {return LoginTime != 0;}
    // 登录次数更新，加 x
    void Loginupd(int x);
    // 获得 ID
    int getID_int() const {return UserID_int;}
    // 输出账户信息 用于调试
    void print() const {
        cout << UserID_int << ' ' << LoginTime << ' ' <<
        UserID << ' ' << Username << ' ' << Password << '\n';
    }
};


void User::userupd() {
    MemoryRiver <User, 0> fileuser("Users");
    fileuser.update(*this, UserID_int);
}
void User::useradd() {
    MemoryRiver<User, 0> fileuser("Users");
    block_list<userstr, int, 0> databaseuser("UserID_to_int");
    std::vector<int> user = databaseuser.find_with_vector(UserID);
    if (user.size() != 0) throw Invalid();
    UserID_int = fileuser.write(*this);
    // cerr << "add : " << UserID_int << '\n';
    fileuser.update(*this, UserID_int);
    databaseuser.insert(UserID, UserID_int);
}

void User::changepassword(string _newpassword) {
    userstr newpassword(_newpassword, isvalidname);
    Password = newpassword;
    userupd();
}

void User::changepassword(string _currentpassword, string _newpassword) {
    userstr currentpassword(_currentpassword, isvalidname),
    newpassword(_newpassword, isvalidname);
    if (!(currentpassword == Password)) throw Invalid();
    Password = newpassword;
    userupd();
}

void User::Loginupd(int x) {
    LoginTime += x;
    userupd();
}

#endif //USER
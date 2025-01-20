#ifndef OPERATOR
#define OPERATOR

#include "MemoryRiver.hpp"
#include "user.hpp"
#include "database.hpp"
#include "mystack.hpp"
#include "mystr.hpp"
#include "book.hpp"
#include "Info.hpp"
#include "Exception.hpp"
#include <string>
#include <vector>

using std::string;
using std::vector;

/*
主要操作：
init 初始化
split_string 将一整行按照空格分开
operation 进行操作
*/
// 初始化
void init();
//将一整行按照空格分开
vector<string> split_string(string s) {
    vector<string> res;
    string now = "";
    for (char ch : s) {
        if (ch == ' ') {
            if (now != "") res.push_back(now);
            now = "";
        }
        else now += ch;
    }
    if (now != "") res.push_back(now);
    return res;
}
// 进行操作
void operation(User &nowuser, string &oprstr);

/*
# 帐户系统指令
su [UserID] ([Password])?
logout
register [UserID] [Password] [Username]
passwd [UserID] ([CurrentPassword])? [NewPassword]
useradd [UserID] [Password] [Privilege] [Username]
delete [UserID]
*/

// 找到 UserID 对应的 UserID_int，如果不存在就抛出异常
int findUser(userstr UserID) {
    block_list<userstr, int, 0> databaseuser("UserID_to_int");
    vector<int> all = databaseuser.find_with_vector(UserID);
    if (all.size() == 0) throw Invalid();
    // assert(all.size() == 1);//一个 UserID 只能对应一个 UserID_int
    return all[0];
}


// 找到 UserID_int 对应的 UserID，如果不存在就抛出异常
User findUser(int UserID_int) {
    User res;
    MemoryRiver<User, 0> fileuser("Users");
    fileuser.read(res, UserID_int);
    return res;
}

// su 登录 传入当前的用户以及指令
void su(User &nowuser, vector<string> orders) {
    if (orders.size() < 2 || orders.size() > 3) throw Invalid();
    userstr UserID(orders[1], isvalidname);
    int UserID_int = findUser(UserID);
    User su_user = findUser(UserID_int);
    if (orders.size() == 3) {
        if (su_user.checkpassword(orders[2]) == false) throw Invalid();
    }
    else if (nowuser.gettype() <= su_user.gettype()) throw Invalid();
    // 可以登录
    su_user.Loginupd(+1);
    nowuser = su_user;
    mystack<int> stackuser("nowUser", 0);
    stackuser.insert(UserID_int);
}
// logout 登出 传入当前的用户以及指令
void logout(User &nowuser, vector<string> orders) {
    if (orders.size() != 1) throw Invalid();
    if (nowuser.gettype() < customer) throw Invalid();
    mystack<int> stackuser("nowUser", 0);
    if (stackuser.size() == 0) throw Invalid();
    nowuser.Loginupd(-1);
    stackuser.pop();
    if (stackuser.size() != 0) {
        int UserID_int = stackuser.top();
        MemoryRiver<User, 0> fileuser;
        fileuser.read(nowuser, UserID_int);
    }
    else nowuser = User();
}
// register 注册 传入当前的用户以及指令
void registeruser(User &nowuser, vector<string> orders) {
    if (orders.size() != 4) throw Invalid();
    User newuser(orders[1], orders[2], orders[3]);
    newuser.useradd();
}
// passwd 修改密码 传入当前的用户以及指令
void passwd(User &nowuser, vector<string> orders) {
    if (orders.size() < 3 || orders.size() > 4) throw Invalid();
    if (nowuser.gettype() < customer) throw Invalid();
    userstr UserID(orders[1], isvalidname);
    int UserID_int = findUser(UserID);
    User upduser = findUser(UserID_int);
    if (orders.size() == 3)
        upduser.changepassword(orders[2]);
    else
        upduser.changepassword(orders[2], orders[3]);
}
// useradd 添加用户 传入当前的用户以及指令
void useradd(User &nowuser, vector<string> orders) {
    if (nowuser.gettype() < employee) throw Invalid();
    if (orders.size() != 5) throw Invalid();
    User newuser(orders[1], orders[2], orders[3], orders[4]);
    if (newuser.gettype() >= nowuser.gettype()) throw Invalid();
    newuser.useradd();
}
// delete 删除用户 传入当前的用户以及指令
void deleteuser(User &nowuser, vector<string> orders) {
    if (orders.size() != 2) throw Invalid();
    if (nowuser.gettype() < owner) throw Invalid();
    // 如果待删除账户已经登录则操作失败
    userstr UserID(orders[1], isvalidname);
    int UserID_int = findUser(UserID);
    User deluser = findUser(UserID_int);
    if (deluser.isLogin()) throw Invalid();
    MemoryRiver<User, 0> fileuser("Users");
    fileuser.Delete(UserID_int);
    block_list<userstr, int, 0> databaseuser("UserID_to_int");
    databaseuser.del(UserID, UserID_int);
}

//初始化
void init() {
    /*
    User 的初始化：Users, UserID_to_int, nowUser
    */
    MemoryRiver<User, 0> fileuser;
    fileuser.initialise("Users", 0, 1);
    block_list<userstr, int, 1> databaseuser("UserID_to_int");
    mystack<int> stackuser("nowUser", 1);
}


// 使用名字前面带下划线来区分
enum operatorType {_Invalid, _exit,// 基础指令
_su, _logout, _register, _passwd, _useradd, _delete, // 账户系统指令
_showbook, _buy, _select, _modify, _import, // 图书系统指令
_showfinance, _log, _reportfinance, _reportemployee, // 日志系统指令
};

operatorType get_opt_type(vector<string> &orders) {
    // 基础指令 _Invalid, _exit
    if (orders.size() == 0) return _Invalid;
    operatorType type = _Invalid;
    if (orders[0] == "exit" || orders[0] == "quit") type = _exit;
    // 账户系统指令 _su, _logout, _register, _passwd, _useradd, _delete
    if (orders[0] == "su") type = _su;
    if (orders[0] == "logout") type = _logout;
    if (orders[0] == "register") type = _register;
    if (orders[0] == "passwd") type = _passwd;
    if (orders[0] == "useradd") type = _useradd;
    if (orders[0] == "delete") type = _delete;
    // 图书系统指令 _showbook, _buy, _select, _modify, _import 
    if (orders[0] == "show") {
        if (orders.size() >= 2 && orders[1] == "finance") type = _showfinance;
        else type = _showbook;
    }
    if (orders[0] == "buy") type = _buy;
    if (orders[0] == "select") type = _select;
    if (orders[0] == "modify") type = _modify;
    if (orders[0] == "import") type = _import;
    // 日志系统指令 _showfinance, _log, _reportfinance, _reportemployee
    if (orders[0] == "log") type = _log;
    if (orders[0] == "report" && orders.size() >= 2) {
        if (orders[1] == "finance") type = _reportfinance;
        if (orders[1] == "employee") type = _reportemployee;
    }
    return type;
}

void operation(User &nowuser, string &optstr) {
    vector<string> orders = split_string(optstr);
    operatorType type = get_opt_type(orders);
    switch (type) {
    case _su:
        
        break;
    
    default:
        throw Invalid();
        break;
    }
}

#endif //OPERATOR
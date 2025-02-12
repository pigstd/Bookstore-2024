#ifndef INFO
#define INFO

#include "mystack.hpp"
#include "user.hpp"
#include "book.hpp"
#include <iostream>
#include <iomanip>

using std::cout;
using std::fixed;
using std::setprecision;


// saleinfo 部分
enum sale_type {_sale = 0, _buyin = 1};

class SaleInfo {
private:
    int SaleID;
    userstr Username;
    bookstr Bookname;
    ISBNstr BookISBN;
    sale_type SaleType;
    int SaleNum;
    ld SaleMoney;
public:
    // 空的构造函数
    SaleInfo();
    // 构造函数
    SaleInfo(int _userID, int _bookID, sale_type _Saletype, int _SaleNum, ld _SaleMoney);
    // 加入数据库 saleInfo
    void addInfo();
    // 输出销售信息 谁买了/卖了什么花了多少钱
    void printInfo() const;
    // 查询金额
    ld queryMoney() const;
    // 查询类型
    sale_type querytype() const;
    // 查询 ID
    int queryID() const;
};

void SaleInfo::addInfo() {
    mystack<SaleInfo> filesaleInfo("saleInfo");
    SaleID = filesaleInfo.insert(*this);
    filesaleInfo.pop();
    filesaleInfo.insert(*this);
}

void SaleInfo::printInfo() const {
    if (SaleType == _buyin)
        cout << fixed << setprecision(2) << "用户 " << Username
        << " 进货了 " << SaleNum << " 本 ISBN 为 " << BookISBN << 
        "，名字为 " << Bookname << " 的书，支出为 " << SaleMoney << "元\n";
    else
        cout << fixed << setprecision(2) << "用户" << Username
        << " 购买了 " << SaleNum << " 本 ISBN 为 " << BookISBN << 
        "，名字为 " << Bookname << " 的书，收入为 " << SaleMoney << "元\n";
}

ld SaleInfo::queryMoney() const {return SaleMoney;}
sale_type SaleInfo::querytype() const {return SaleType;}

SaleInfo::SaleInfo() {};
SaleInfo::SaleInfo(int _userID, int _bookID, sale_type _Saletype, int _SaleNum, ld _SaleMoney) :
    SaleID(-1), SaleType(_Saletype), SaleNum(_SaleNum), SaleMoney(_SaleMoney) {
    User user = findUser(_userID);
    Book book = find_with_BookID(_bookID);
    Username = user.getname();
    Bookname = book.queryname();
    BookISBN = book.queryISBN();
}

int SaleInfo::queryID() const {return SaleID;}

// logInfo 部分

// operator 类型：使用名字前面带下划线来区分
enum operatorType {_Invalid, _quit,// 基础指令
_su, _logout, _register, _passwd, _useradd, _delete, // 账户系统指令
_showbook, _buy, _select, _modify, _import, // 图书系统指令
_showfinance, _log, _reportfinance, _reportemployee, // 日志系统指令
_do_noting // 只有空格的指令，无输出内容
};

// 操作的信息，长度最多为 150
#define logstr str<150>

class logInfo {
private:
    operatorType type;
    userstr Username;
    bookstr Bookname;
    ISBNstr BookISBN;
    bool isemployee;
    int saleinfoID;
    userstr object_username;
    logstr operator_info;
public:
    // 输出日志信息
    void show() const {
        switch (type)
        {
        case _reportfinance:
            cout << "超级管理员生成财务记录报告\n";
            break;
        case _reportemployee:
            cout << "超级管理员生成员工工作报告\n";
            break;
        case _log:
            cout << "超级管理员生成日志\n";
            break;
        case _su:
            cout << "用户 " << object_username << " 登录\n";
            break;
        case _logout:
            cout << "用户 " << object_username << " 注销\n";
            break;
        case _delete:
            cout << "用户 " << object_username << " 被删除\n";
            break;
        case _passwd:
            cout << "用户 " << Username << " 修改了用户 "
            << object_username << " 的密码为 " << operator_info << "\n";
            break;
        case _register:
            cout << "用户 " << Username << " 注册了用户 " << object_username
            << "," << operator_info << '\n';
            break;
        case _useradd:
            cout << "用户 " << Username << " 创建了用户 " << object_username
            << "," << operator_info << '\n';
            break;
        case _showbook:
            if (string(operator_info) == "")
                cout << "用户 " << Username << " 检索全部图书\n";
            else
                cout << "用户 " << Username << " 检索 " << operator_info << " 的图书\n";
            break;
        case _buy: {
            MemoryRiver<SaleInfo> filesaleInfo("saleInfo");
            SaleInfo info; filesaleInfo.read(info, saleinfoID);
            info.printInfo();
            break;
        }
        case _modify:
            cout << "用户 " << Username << " 修改了 ISBN 为 " << BookISBN <<
            " 的图书信息：" << operator_info << "\n";
            break;
        case _import: {
            MemoryRiver<SaleInfo> filesaleInfo("saleInfo");
            SaleInfo info; filesaleInfo.read(info, saleinfoID);
            info.printInfo();
            break;
        }
        case _select:
            cout << "用户 " << Username << " 选择了 ISBN 为" << BookISBN << " 的图书\n";
            break;
        case _showfinance: {
            if (string(operator_info) == "")
               cout << "超级管理员查询了所有交易总额\n";
            else cout << "超级管理员查询了最后完成的 " <<
            operator_info << "笔交易总额";
            break;
        }
        case _quit:
            cout << "退出系统\n";
            break;
        default:
            break;
        }
    }
    // 如果是员工就show，否则什么都不干
    void showemployee() const {
        if (isemployee) show();
    }
    // 空的构造函数
    logInfo() {}
    // report finance, report emplyee, log, quit
    // 只需要知道操作类型的操作：
    logInfo(operatorType _type):type(_type), isemployee(false) {
        assert(_type == _reportfinance || _type == _reportemployee || _type == _log || _type == _quit);
    }
    // su 登录
    // logout 注销
    // delete 删除
    // 需要知道登录/注销/删除的账户名字，以及权限
    logInfo(operatorType _type, usertype _usertype, userstr _username):type(_type), object_username(_username) {
        assert(_type == _su || _type == _logout || _type == _delete);
        isemployee = (_usertype == employee);
    }
    // register 注册账户
    // passwd 修改密码
    // useradd 创建账户
    // 需要知道操作的人，操作的对象和操作的信息
    logInfo(operatorType _type, usertype _usertype, userstr _username, userstr _objectusername, string info):
    type(_type), Username(_username), object_username(_objectusername), operator_info(info) {
        assert(_type == _register || _type == _passwd || _type == _useradd);
        isemployee = (_usertype == employee);
    }
    // show 检索图书
    // 需要知道操作用户的名字，操作的信息
    logInfo(operatorType _type, usertype _usertype, userstr _username, string info):
    type(_type), Username(_username), operator_info(info) {
        assert(_type == _showbook);
        isemployee = (_usertype == employee);
    }
    // buy 购买图书
    // import 进货
    // 只需要记录 saleID
    logInfo(operatorType _type, usertype _usertype, int _saleinfoID):
    type(_type), saleinfoID(_saleinfoID) {
        assert(_type == _buy || _type == _import);
        isemployee = (_usertype == employee);
    }
    // modify 修改图书信息
    // 需要知道操作用户的名字，图书的信息，操作的信息
    logInfo(operatorType _type, usertype _usertype, userstr _username, ISBNstr _BookISBN, string info):
    type(_type), Username(_username), BookISBN(_BookISBN), operator_info(info) {
        assert(_type == _modify);
        isemployee = (_usertype == employee);
    }
    // select 选择图书
    // 需要知道操作用户的名字，图书的 ISBN
    logInfo(operatorType _type, usertype _usertype, userstr _username, ISBNstr _BookISBN):
    type(_type), Username(_username), BookISBN(_BookISBN) {
        assert(_type == _select);
        isemployee = (_usertype == employee);
    }
    // show finance 财务记录查询
    // 需要知道操作的信息
    logInfo(operatorType _type, string info): type(_type), operator_info(info) {
        if (_type != _showfinance) cerr << _type << '\n';
        assert(_type == _showfinance);
        isemployee = false;
    }
    // 加入数据库中
    void add() const {
        mystack<logInfo> filelogInfo("logInfo");
        filelogInfo.insert(*this);
    }
};

#endif //INFO
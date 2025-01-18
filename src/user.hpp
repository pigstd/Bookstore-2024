#ifndef USER
#define USER

enum usertype {
    visitor = 0,
    customer = 1,
    employee = 3,
    owner = 7
};

class User {
private:
    int UserID_int;
    usertype userType;
    char userID[31], userName[31], userPassword[31];
public:
};

#endif //USER
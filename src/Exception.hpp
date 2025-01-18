#ifndef EXCEPTION
#define EXCEPTION
#include<exception>

class Invalid : public std::exception {
public:
    virtual char * what() const noexcept override {
        return "Invalid";
    }
};

#endif //EXCEPTION
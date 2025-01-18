#ifndef EXCEPTION
#define EXCEPTION
#include <exception>

class Invalid : public std::exception {
public:
    virtual const char * what() const noexcept override {
        return "Invalid";
    }
};

#endif //EXCEPTION
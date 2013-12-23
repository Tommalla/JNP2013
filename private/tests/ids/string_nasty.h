#ifndef STRING_NASTY_H
#define STRING_NASTY_H

#include <string>
#include "../exceptional.h"

template<unsigned int prob>
struct StringNasty {
    string x;
    StringNasty() {
       Exceptional<prob>::mess(); 
    }
    StringNasty(const char* _x) : x(_x) {
       Exceptional<prob>::mess(); 
    }
    StringNasty(StringNasty const &other) : x(other.x) {
       Exceptional<prob>::mess();
    }
    bool operator<= (StringNasty const &rhs) const {
        Exceptional<prob>::mess();
        return x <= rhs.x;
    }
    bool operator>= (StringNasty const &rhs) const {
        return rhs <= *this;
    }
    bool operator== (StringNasty const &rhs) const {
        Exceptional<prob>::mess();
        return x == rhs.x;
    }
    bool operator!= (StringNasty const &rhs) const {
        return !(*this == rhs);
    }
    bool operator< (StringNasty const &rhs) const {
        return (*this <= rhs) && (*this != rhs);
    }
    bool operator> (StringNasty const &rhs) const {
        return rhs < *this;
    }
    bool operator== (const char * const rhs) const {
        return x == string(rhs);
    }
};

#endif

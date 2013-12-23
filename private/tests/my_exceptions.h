#ifndef EXCEPTION_H
#define EXCEPTION_H
class RandomException {
    static const char* what() noexcept
    {
        return "RandomException";
    }
};
class TimeoutException {
    static const char* what() noexcept
    {
        return "TimeoutException";
    }
};
#endif

#include "Exception.h"

Exception::Exception(int exception_code) : code_(exception_code)
{
}

Exception::~Exception() //throw()
{
}

const char* Exception::what() const throw()
{
    return error_message_[static_cast<int>(code_)].c_str();
}

int Exception::getCode()
{
    return code_;
}

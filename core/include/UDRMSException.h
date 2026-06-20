#ifndef UDRMSEXCEPTION_H
#define UDRMSEXCEPTION_H

#include <stdexcept>
using std::runtime_error;

class UDRMSException: public runtime_error{
    public:
        UDRMSException(const std::string message);
};

#endif
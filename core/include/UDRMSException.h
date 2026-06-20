#ifndef UDRMSEXCEPTION_H
#define UDRMSEXCEPTION_H

#include <stdexcept>
#include <string>
class UDRMSException: public std::runtime_error{
    public:
        UDRMSException(const std::string message);
};

#endif
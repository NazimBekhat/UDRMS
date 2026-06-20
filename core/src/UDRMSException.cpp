#include "UDRMSException.h"

UDRMSException::UDRMSException(const std::string message): std::runtime_error(message){}
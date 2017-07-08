#include "UtilException.h"

using namespace CipherKick;


UtilException::UtilException()
	: std::exception()
{
	message = "";
}

UtilException::UtilException(const std::string& message)
	: std::exception(), message(message) { }

UtilException::UtilException(const std::string& message, const std::exception& cause)
	: std::exception(cause), message(message) { }

